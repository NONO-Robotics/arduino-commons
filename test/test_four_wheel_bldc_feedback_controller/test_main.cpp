#include <unity.h>

#include "NativeTestCommon.h"

void test_feedback_controller_bounds_correction_with_feedback() {
  // Prepare
  resetMocks();
  FourWheelBLDCFeedbackController controller(
      4.0F, 10, 100, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
      1.0F, 1.0F, 10.0F, 1.0F, 0.5F, 1.0F);
  FourWheelAngularSpeed command;
  command.updateFrom(2.0F, 2.0F, 2.0F, 2.0F);
  controller.setFrontLeftFeedback(-10.0F);
  controller.applySpeed(command);
  arduino_mock::now_ms = 100;

  // Perform
  controller.applySpeed(command);

  // Asserts
  TEST_ASSERT_LESS_OR_EQUAL_INT(100, arduino_mock::pwm_values[34]);
  TEST_ASSERT_GREATER_OR_EQUAL_INT(0, arduino_mock::pwm_values[34]);
}

void test_feedback_controller_bounds_negative_target_correction() {
  // Prepare
  resetMocks();
  FourWheelBLDCFeedbackController controller(
      4.0F, 10, 100, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
      1.0F, 1.0F, 10.0F, 1.0F, 0.5F, 1.0F);
  FourWheelAngularSpeed command;
  command.updateFrom(2.0F, 2.0F, 2.0F, 2.0F);
  controller.setFrontLeftFeedback(-10.0F);
  controller.applySpeed(command);
  arduino_mock::now_ms = 100;
  controller.applySpeed(command);
  command.setFlWInRad(0.0F);
  controller.applySpeed(command);
  command.setFlWInRad(-2.0F);
  controller.setFrontLeftFeedback(10.0F);
  arduino_mock::now_ms = 200;

  // Perform
  controller.applySpeed(command);

  // Asserts
  TEST_ASSERT_LESS_OR_EQUAL_INT(100, arduino_mock::pwm_values[34]);
  TEST_ASSERT_GREATER_OR_EQUAL_INT(0, arduino_mock::pwm_values[34]);
}

void test_feedback_controller_stop_engages_brakes() {
  // Prepare
  resetMocks();
  FourWheelBLDCFeedbackController controller(
      4.0F, 10, 100, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
      1.0F, 1.0F, 10.0F, 1.0F, 0.5F, 1.0F);
  FourWheelAngularSpeed command;
  command.updateFrom(2.0F, 2.0F, 2.0F, 2.0F);
  controller.setFrontLeftFeedback(-10.0F);
  controller.applySpeed(command);

  // Perform
  controller.stop();

  // Asserts
  TEST_ASSERT_EQUAL_INT(HIGH, arduino_mock::digital_values[33]);
  TEST_ASSERT_EQUAL_INT(HIGH, arduino_mock::digital_values[36]);
  TEST_ASSERT_EQUAL_INT(HIGH, arduino_mock::digital_values[39]);
  TEST_ASSERT_EQUAL_INT(HIGH, arduino_mock::digital_values[42]);
}

void test_feedback_controller_without_feedback_passes_target_through() {
  // Prepare
  resetMocks();
  FourWheelBLDCFeedbackController controller(
      4.0F, 10, 100, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
      1.0F, 1.0F, 10.0F, 1.0F, 0.5F, 1.0F);
  FourWheelAngularSpeed command;
  command.updateFrom(1.0F, 1.0F, 1.0F, 1.0F);
  controller.setFrontLeftFeedback(-10.0F);
  controller.invalidateFrontLeftFeedback();

  // Perform
  controller.applySpeed(command);

  // Asserts
  TEST_ASSERT_GREATER_THAN(0, arduino_mock::pwm_values[34]);
}

void test_feedback_controller_negative_max_w_configures_pins() {
  // Prepare
  resetMocks();
  FourWheelBLDCFeedbackController negativeMaxW(
      -10.0F, 10, 100, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62,
      1.0F, 1.0F, 10.0F, 1.0F, 0.5F, 1.0F);

  // Perform

  // Asserts
  TEST_ASSERT_EQUAL_INT(OUTPUT, arduino_mock::pin_modes[51]);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_feedback_controller_bounds_correction_with_feedback);
  RUN_TEST(test_feedback_controller_bounds_negative_target_correction);
  RUN_TEST(test_feedback_controller_stop_engages_brakes);
  RUN_TEST(test_feedback_controller_without_feedback_passes_target_through);
  RUN_TEST(test_feedback_controller_negative_max_w_configures_pins);
  return UNITY_END();
}
