#include <memory>

#include <unity.h>

#include "NativeTestCommon.h"
#include "OutdoorFourWheelFeedbackController.h"
#include "WheelSpeeds.h"

namespace {

constexpr float kMinW = 0.01f;
constexpr unsigned long kFeedbackTimeoutMs = 150;
constexpr int kFrontLeftPwmPin = 14;

WheelFeedbackSettings passthrough() {
  return {false, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f};
}

WheelFeedbackSettings frontLeftFeedback(float speedScale,
                                        float proportionalGain) {
  return {true, speedScale, proportionalGain, 0.0f, 1.0f, 1.0f};
}

std::unique_ptr<OutdoorFourWheelFeedbackController> makeControllerWith(
    const WheelFeedbackSettings &frontRight,
    const WheelFeedbackSettings &frontLeft,
    const WheelFeedbackSettings &backRight,
    const WheelFeedbackSettings &backLeft) {
  return std::unique_ptr<OutdoorFourWheelFeedbackController>(
      new OutdoorFourWheelFeedbackController(
          kMinW, kFeedbackTimeoutMs, settings(11, 12, 13, 0, true),
          settings(14, 15, 16, 1), settings(17, 18, 19, 2, true),
          settings(20, 21, 22, 3), frontRight, frontLeft, backRight,
          backLeft));
}

std::unique_ptr<OutdoorFourWheelFeedbackController> makeController(
    float frontLeftSpeedScale, float frontLeftGain) {
  return makeControllerWith(passthrough(),
                            frontLeftFeedback(frontLeftSpeedScale,
                                              frontLeftGain),
                            passthrough(), passthrough());
}

std::unique_ptr<OutdoorFourWheelFeedbackController> makeControllerAllEnabled(
    float speedScale, float gain) {
  const WheelFeedbackSettings enabled =
      frontLeftFeedback(speedScale, gain);
  return makeControllerWith(enabled, enabled, enabled, enabled);
}

std::unique_ptr<OutdoorFourWheelFeedbackController> makeControllerAllDisabled() {
  return makeControllerWith(passthrough(), passthrough(), passthrough(),
                            passthrough());
}

FourWheelAngularSpeed allTargets(float target) {
  FourWheelAngularSpeed command;
  command.updateFrom(target, target, target, target);
  return command;
}

FourWheelAngularSpeed frontLeftTarget(float target) {
  FourWheelAngularSpeed command;
  command.updateFrom(target, 0.0f, 0.0f, 0.0f);
  return command;
}

WheelSpeeds onlyFrontLeft(float frontLeft) {
  return WheelSpeeds(0.0f, 0.0f, frontLeft, 0.0f, 0.0f, 0.0f);
}

void feedFreshFeedback(OutdoorFourWheelFeedbackController *controller,
                       float measuredFrontLeft) {
  controller->receiveFreshCommand();
  arduino_mock::now_ms = 100;
  controller->updateFeedback(onlyFrontLeft(measuredFrontLeft));
}

void feedFreshFeedbackForAllWheels(OutdoorFourWheelFeedbackController *controller) {
  controller->receiveFreshCommand();
  arduino_mock::now_ms = 100;
  controller->updateFeedback(WheelSpeeds(0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f));
}

float applyAndReadFrontLeftPwm(float speedScale, float gain,
                               float measuredFrontLeft) {
  resetMocks();
  auto controller = makeController(speedScale, gain);
  feedFreshFeedback(controller.get(), measuredFrontLeft);

  TEST_ASSERT_TRUE(controller->applySpeed(frontLeftTarget(1.0f)));
  return static_cast<float>(arduino_mock::pwm_values[kFrontLeftPwmPin]);
}

bool brakesEngaged() {
  return arduino_mock::digital_values[13] == HIGH &&
         arduino_mock::digital_values[16] == HIGH &&
         arduino_mock::digital_values[19] == HIGH &&
         arduino_mock::digital_values[22] == HIGH;
}

void assertAllBrakesEngaged() { TEST_ASSERT_TRUE(brakesEngaged()); }

}  // namespace

void test_apply_speed_drives_every_wheel_when_feedback_is_disabled() {
  // Prepare
  resetMocks();
  auto controller = makeControllerAllDisabled();
  FourWheelAngularSpeed command;
  command.updateFrom(2.0f, -2.0f, -1.0f, 1.0f);

  // Perform
  const bool applied = controller->applySpeed(command);

  // Asserts
  TEST_ASSERT_TRUE(applied);
  TEST_ASSERT_GREATER_THAN(0, arduino_mock::pwm_values[11]);
  TEST_ASSERT_GREATER_THAN(0, arduino_mock::pwm_values[14]);
  TEST_ASSERT_GREATER_THAN(0, arduino_mock::pwm_values[17]);
  TEST_ASSERT_GREATER_THAN(0, arduino_mock::pwm_values[20]);
}

void test_enabled_feedback_without_measurements_brakes_every_wheel() {
  // Prepare
  resetMocks();
  auto controller = makeController(1.0f, 0.5f);

  // Perform
  const bool applied = controller->applySpeed(frontLeftTarget(1.0f));

  // Asserts
  TEST_ASSERT_FALSE(applied);
  assertAllBrakesEngaged();
}

void test_fresh_command_and_measurements_unlock_feedback() {
  // Prepare
  resetMocks();
  auto controller = makeController(1.0f, 0.5f);
  feedFreshFeedback(controller.get(), 1.0f);

  // Perform
  const bool applied = controller->applySpeed(frontLeftTarget(1.0f));

  // Asserts
  TEST_ASSERT_TRUE(applied);
}

void test_stale_measurements_brake_every_wheel() {
  // Prepare
  resetMocks();
  auto controller = makeController(1.0f, 0.5f);
  feedFreshFeedback(controller.get(), 1.0f);
  arduino_mock::now_ms = 100 + kFeedbackTimeoutMs + 1;

  // Perform
  const bool applied = controller->applySpeed(frontLeftTarget(1.0f));

  // Asserts
  TEST_ASSERT_FALSE(applied);
  assertAllBrakesEngaged();
}

void test_fault_all_feedback_recovers_only_after_fresh_inputs() {
  // Prepare
  resetMocks();
  auto controller = makeController(1.0f, 0.5f);
  feedFreshFeedback(controller.get(), 1.0f);
  TEST_ASSERT_TRUE(controller->applySpeed(frontLeftTarget(1.0f)));

  // Perform
  controller->faultAllFeedback();
  const bool appliedWhileFaulted = controller->applySpeed(frontLeftTarget(1.0f));
  const bool brakesEngagedWhileFaulted = brakesEngaged();
  feedFreshFeedback(controller.get(), 1.0f);
  const bool appliedAfterRecovery = controller->applySpeed(frontLeftTarget(1.0f));

  // Asserts
  TEST_ASSERT_FALSE(appliedWhileFaulted);
  TEST_ASSERT_TRUE(brakesEngagedWhileFaulted);
  TEST_ASSERT_TRUE(appliedAfterRecovery);
}

void test_fault_feedback_brakes_for_every_wheel_position() {
  // Prepare
  resetMocks();
  auto controller = makeControllerAllEnabled(1.0f, 0.5f);
  const WheelPosition wheels[] = {WheelPosition::FrontRight,
                                  WheelPosition::FrontLeft,
                                  WheelPosition::BackRight,
                                  WheelPosition::BackLeft};
  const FourWheelAngularSpeed command = allTargets(1.0f);

  // Perform — every channel is exercised and re-armed between rounds
  for (WheelPosition wheel : wheels) {
    feedFreshFeedbackForAllWheels(controller.get());
    TEST_ASSERT_TRUE(controller->applySpeed(command));
    controller->faultFeedback(wheel);
    TEST_ASSERT_FALSE(controller->applySpeed(command));
  }

  // Asserts
  assertAllBrakesEngaged();
}

void test_stop_engages_every_brake() {
  // Prepare
  resetMocks();
  auto controller = makeController(1.0f, 0.5f);
  feedFreshFeedback(controller.get(), 1.0f);
  TEST_ASSERT_TRUE(controller->applySpeed(frontLeftTarget(1.0f)));

  // Perform
  controller->stop();

  // Asserts
  assertAllBrakesEngaged();
}

void test_wheel_speed_scale_normalizes_measurement_and_pi_reacts() {
  // Prepare — reference run where PI error is exactly zero
  const float noErrorPwm = applyAndReadFrontLeftPwm(1.0f, 0.5f, 1.0f);

  // Perform — same physical speed expressed with a different scale
  const float scaledPwm = applyAndReadFrontLeftPwm(2.0f, 0.5f, 2.0f);

  // Perform — wheel stopped while a nonzero target is commanded
  const float stoppedPwm = applyAndReadFrontLeftPwm(1.0f, 0.5f, 0.0f);

  // Asserts
  TEST_ASSERT_EQUAL_INT(noErrorPwm, scaledPwm);
  TEST_ASSERT_GREATER_THAN(noErrorPwm, stoppedPwm);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_apply_speed_drives_every_wheel_when_feedback_is_disabled);
  RUN_TEST(test_enabled_feedback_without_measurements_brakes_every_wheel);
  RUN_TEST(test_fresh_command_and_measurements_unlock_feedback);
  RUN_TEST(test_stale_measurements_brake_every_wheel);
  RUN_TEST(test_fault_all_feedback_recovers_only_after_fresh_inputs);
  RUN_TEST(test_fault_feedback_brakes_for_every_wheel_position);
  RUN_TEST(test_stop_engages_every_brake);
  RUN_TEST(test_wheel_speed_scale_normalizes_measurement_and_pi_reacts);
  return UNITY_END();
}
