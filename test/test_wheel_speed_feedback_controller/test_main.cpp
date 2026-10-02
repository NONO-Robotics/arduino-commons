#include <unity.h>

#include "NativeTestCommon.h"
#include "WheelSpeedFeedbackController.h"

namespace {

WheelFeedbackSettings enabledSettings(float proportionalGain,
                                      float integralGain) {
  return {true, 1.0f, proportionalGain, integralGain, 10.0f, 10.0f};
}

}  // namespace

void test_disabled_channel_is_always_safe_and_passthrough() {
  // Prepare
  resetMocks();
  WheelSpeedFeedbackController controller({false, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f});

  // Perform

  // Asserts
  TEST_ASSERT_TRUE(controller.canSafelyUseFeedback(1.0f, 100, 150));
  TEST_ASSERT_EQUAL_FLOAT(1.0f, controller.correctTarget(1.0f, 0.01f, 1.0f, 100));
}

void test_enabled_channel_needs_fresh_command_and_wheel_speeds() {
  // Prepare
  resetMocks();
  WheelSpeedFeedbackController controller(enabledSettings(0.0f, 0.0f));

  // Perform

  // Asserts
  TEST_ASSERT_FALSE(controller.canSafelyUseFeedback(1.0f, 100, 150));
  controller.receiveFreshCommand();
  TEST_ASSERT_FALSE(controller.canSafelyUseFeedback(1.0f, 100, 150));
  controller.updateFeedback(1.0f, 100);
  TEST_ASSERT_TRUE(controller.canSafelyUseFeedback(1.0f, 100, 150));
}

void test_enabled_channel_rejects_stale_wheel_speeds() {
  // Prepare
  resetMocks();
  WheelSpeedFeedbackController controller(enabledSettings(0.0f, 0.0f));
  controller.receiveFreshCommand();
  controller.updateFeedback(1.0f, 100);

  // Perform

  // Asserts
  TEST_ASSERT_TRUE(controller.canSafelyUseFeedback(1.0f, 250, 150));
  TEST_ASSERT_FALSE(controller.canSafelyUseFeedback(1.0f, 251, 150));
}

void test_zero_target_is_safe_before_any_input_arrives() {
  // Prepare
  resetMocks();
  WheelSpeedFeedbackController controller(enabledSettings(0.0f, 0.0f));

  // Perform

  // Asserts
  TEST_ASSERT_TRUE(controller.canSafelyUseFeedback(0.0f, 100, 150));
}

void test_stale_feedback_faults_and_requires_both_inputs_again() {
  // Prepare
  resetMocks();
  WheelSpeedFeedbackController controller(enabledSettings(0.0f, 0.0f));
  controller.receiveFreshCommand();
  controller.updateFeedback(1.0f, 100);

  // Perform — stale measurement latches a fault
  TEST_ASSERT_FALSE(controller.canSafelyUseFeedback(1.0f, 251, 150));

  // Asserts — a fresh command alone does not recover the channel
  controller.receiveFreshCommand();
  TEST_ASSERT_FALSE(controller.canSafelyUseFeedback(1.0f, 252, 150));
  controller.updateFeedback(1.0f, 252);
  TEST_ASSERT_TRUE(controller.canSafelyUseFeedback(1.0f, 252, 150));
}

void test_fault_requires_fresh_command_and_wheel_speeds() {
  // Prepare
  resetMocks();
  WheelSpeedFeedbackController controller(enabledSettings(0.0f, 0.0f));
  controller.receiveFreshCommand();
  controller.updateFeedback(1.0f, 100);

  // Perform
  controller.fault();

  // Asserts
  TEST_ASSERT_FALSE(controller.canSafelyUseFeedback(1.0f, 100, 150));
  controller.receiveFreshCommand();
  TEST_ASSERT_FALSE(controller.canSafelyUseFeedback(1.0f, 100, 150));
  controller.updateFeedback(1.0f, 100);
  TEST_ASSERT_TRUE(controller.canSafelyUseFeedback(1.0f, 100, 150));
}

void test_correct_target_without_feedback_passes_target_through() {
  // Prepare
  resetMocks();
  WheelSpeedFeedbackController controller(enabledSettings(1.0f, 0.0f));
  controller.receiveFreshCommand();

  // Perform

  // Asserts
  TEST_ASSERT_EQUAL_FLOAT(0.5f,
                          controller.correctTarget(0.5f, 0.01f, 1.0f, 100));
}

void test_correct_target_clamps_positive_result_to_max_w() {
  // Prepare
  resetMocks();
  WheelSpeedFeedbackController controller(enabledSettings(1.0f, 0.0f));
  controller.receiveFreshCommand();
  controller.updateFeedback(0.0f, 100);

  // Perform — PI overshoots past maxW
  const float corrected = controller.correctTarget(0.5f, 0.01f, 0.6f, 100);

  // Asserts
  TEST_ASSERT_EQUAL_FLOAT(0.6f, corrected);
}

void test_correct_target_floors_positive_result_to_min_w() {
  // Prepare
  resetMocks();
  WheelSpeedFeedbackController controller(enabledSettings(1.0f, 0.0f));
  controller.receiveFreshCommand();
  controller.updateFeedback(1.0f, 100);

  // Perform — PI drives the target below the usable torque floor
  const float corrected = controller.correctTarget(0.5f, 0.01f, 1.0f, 100);

  // Asserts
  TEST_ASSERT_EQUAL_FLOAT(0.01f, corrected);
}

void test_correct_target_clamps_negative_result_to_minus_max_w() {
  // Prepare
  resetMocks();
  WheelSpeedFeedbackController controller(enabledSettings(1.0f, 0.0f));
  controller.receiveFreshCommand();
  controller.updateFeedback(0.0f, 100);

  // Perform — reverse command with a stopped wheel
  const float corrected = controller.correctTarget(-0.5f, 0.01f, 0.6f, 100);

  // Asserts
  TEST_ASSERT_EQUAL_FLOAT(-0.6f, corrected);
}

void test_correct_target_floors_negative_result_to_minus_min_w() {
  // Prepare
  resetMocks();
  WheelSpeedFeedbackController controller(enabledSettings(1.0f, 0.0f));
  controller.receiveFreshCommand();
  controller.updateFeedback(-0.5f, 100);

  // Perform — reverse target below the usable torque floor
  const float corrected = controller.correctTarget(-0.5f, 1.0f, 2.0f, 100);

  // Asserts
  TEST_ASSERT_EQUAL_FLOAT(-1.0f, corrected);
}

void test_correct_target_resets_integrator_on_zero_target() {
  // Prepare
  resetMocks();
  WheelSpeedFeedbackController controller(enabledSettings(0.0f, 5.0f));
  controller.receiveFreshCommand();
  controller.updateFeedback(0.0f, 100);
  controller.correctTarget(0.5f, 0.01f, 10.0f, 100);
  TEST_ASSERT_EQUAL_FLOAT(
      3.0f, controller.correctTarget(0.5f, 0.01f, 10.0f, 1100));

  // Perform — zero target clears the accumulated PI error
  TEST_ASSERT_EQUAL_FLOAT(
      0.0f, controller.correctTarget(0.0f, 0.01f, 10.0f, 1200));

  // Asserts — integral restarts from zero instead of resuming
  TEST_ASSERT_EQUAL_FLOAT(
      0.5f, controller.correctTarget(0.5f, 0.01f, 10.0f, 1300));
}

void test_integral_accumulates_over_elapsed_time() {
  // Prepare
  resetMocks();
  WheelSpeedFeedbackController controller(enabledSettings(0.0f, 1.0f));
  controller.receiveFreshCommand();
  controller.updateFeedback(0.0f, 100);
  controller.correctTarget(0.5f, 0.01f, 100.0f, 100);

  // Perform — one second of accumulated error
  const float corrected = controller.correctTarget(0.5f, 0.01f, 100.0f, 1100);

  // Asserts
  TEST_ASSERT_EQUAL_FLOAT(1.0f, corrected);
}

void test_reset_clears_accumulated_integral() {
  // Prepare
  resetMocks();
  WheelSpeedFeedbackController controller(enabledSettings(0.0f, 1.0f));
  controller.receiveFreshCommand();
  controller.updateFeedback(0.0f, 100);
  controller.correctTarget(0.5f, 0.01f, 100.0f, 100);
  controller.correctTarget(0.5f, 0.01f, 100.0f, 1100);

  // Perform
  controller.reset();

  // Asserts — no elapsed interval on the first call after reset
  TEST_ASSERT_EQUAL_FLOAT(0.5f,
                          controller.correctTarget(0.5f, 0.01f, 100.0f, 2000));
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_disabled_channel_is_always_safe_and_passthrough);
  RUN_TEST(test_enabled_channel_needs_fresh_command_and_wheel_speeds);
  RUN_TEST(test_enabled_channel_rejects_stale_wheel_speeds);
  RUN_TEST(test_zero_target_is_safe_before_any_input_arrives);
  RUN_TEST(test_stale_feedback_faults_and_requires_both_inputs_again);
  RUN_TEST(test_fault_requires_fresh_command_and_wheel_speeds);
  RUN_TEST(test_correct_target_without_feedback_passes_target_through);
  RUN_TEST(test_correct_target_clamps_positive_result_to_max_w);
  RUN_TEST(test_correct_target_floors_positive_result_to_min_w);
  RUN_TEST(test_correct_target_clamps_negative_result_to_minus_max_w);
  RUN_TEST(test_correct_target_floors_negative_result_to_minus_min_w);
  RUN_TEST(test_correct_target_resets_integrator_on_zero_target);
  RUN_TEST(test_integral_accumulates_over_elapsed_time);
  RUN_TEST(test_reset_clears_accumulated_integral);
  return UNITY_END();
}
