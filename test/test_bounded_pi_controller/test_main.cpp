#include <cassert>
#include <unity.h>

#include "BoundedPIController.h"
#include "NativeTestCommon.h"

static bool closeEnough(float left, float right) {
  const float difference = left - right;
  return difference < 0.0001f && difference > -0.0001f;
}

void test_bounded_pi_clamps_positive_error_to_max_correction() {
  // Prepare
  BoundedPIController controller(0.5f, 0.0f, 1.0f, 10.0f);

  // Perform

  // Asserts
  TEST_ASSERT_TRUE(closeEnough(controller.update(2.0f, 0.0f, 0.0f), 1.0f));
}

void test_bounded_pi_clamps_negative_error() {
  // Prepare
  BoundedPIController controller(0.5f, 0.0f, 1.0f, 10.0f);

  // Perform

  // Asserts
  TEST_ASSERT_TRUE(closeEnough(controller.update(-2.0f, 0.0f, 0.0f), -1.0f));
}

void test_bounded_pi_integral_step_size_matches() {
  // Prepare
  BoundedPIController frequent(0.0f, 1.0f, 10.0f, 10.0f);
  BoundedPIController infrequent(0.0f, 1.0f, 10.0f, 10.0f);
  for (int index = 0; index < 10; ++index) {
    frequent.update(1.0f, 0.0f, 0.001f);
  }

  // Perform

  // Asserts
  TEST_ASSERT_TRUE(closeEnough(infrequent.update(1.0f, 0.0f, 0.010f),
                               frequent.update(1.0f, 0.0f, 0.0f)));
}

void test_bounded_pi_custom_lower_bound_clamps_negative() {
  // Prepare
  BoundedPIController directional(1.0f, 0.0f, 10.0f, 10.0f);

  // Perform

  // Asserts
  TEST_ASSERT_TRUE(closeEnough(
      directional.update(0.1f, 10.0f, 0.0f, -0.1f, 0.9f), -0.1f));
}

void test_bounded_pi_custom_upper_bound_clamps_positive() {
  // Prepare
  BoundedPIController directional(1.0f, 0.0f, 10.0f, 10.0f);

  // Perform

  // Asserts
  TEST_ASSERT_TRUE(closeEnough(
      directional.update(-0.1f, -10.0f, 0.0f, -0.9f, 0.1f), 0.1f));
}

void test_bounded_pi_holds_integral_when_saturated() {
  // Prepare
  BoundedPIController saturated(0.0f, 1.0f, 1.0f, 10.0f);

  // Perform

  // Asserts
  TEST_ASSERT_TRUE(closeEnough(
      saturated.update(2.0f, 0.0f, 1.0f, -1.0f, 0.2f), 0.2f));
  TEST_ASSERT_TRUE(closeEnough(saturated.update(0.0f, 0.0f, 1.0f), 0.0f));
}

void test_bounded_pi_reset_clears_integral() {
  // Prepare
  BoundedPIController saturated(0.0f, 1.0f, 1.0f, 10.0f);
  saturated.update(2.0f, 0.0f, 1.0f, -1.0f, 0.2f);

  // Perform
  saturated.reset();

  // Asserts
  TEST_ASSERT_TRUE(closeEnough(saturated.update(1.0f, 0.0f, 0.0f), 0.0f));
}

void test_bounded_pi_negative_ctor_limits_absolutized() {
  // Prepare
  BoundedPIController negativeBounds(1.0f, 1.0f, -0.5f, -1.0f);

  // Perform

  // Asserts
  TEST_ASSERT_TRUE(closeEnough(negativeBounds.update(10.0f, 0.0f, 1.0f), 0.5f));
}

void test_bounded_pi_releases_saturation_with_negative_error() {
  // Prepare
  BoundedPIController antiWindup(1.0f, 1.0f, 0.5f, 1.0f);

  // Perform

  // Asserts
  TEST_ASSERT_TRUE(closeEnough(antiWindup.update(-10.0f, 0.0f, 1.0f), -0.5f));
}

void test_bounded_pi_allowed_drift_when_clamped_high() {
  // Prepare
  BoundedPIController allowedDrift(1.0f, 0.0f, 10.0f, 10.0f);

  // Perform

  // Asserts
  TEST_ASSERT_TRUE(closeEnough(
      allowedDrift.update(0.0f, 0.5f, 1.0f, -5.0f, -1.0f), -1.0f));
}

void test_bounded_pi_integrates_negative_then_clamps_with_positive_error() {
  // Prepare
  BoundedPIController wideThenNarrow(0.0f, 1.0f, 100.0f, 100.0f);
  for (int index = 0; index < 12; ++index) {
    wideThenNarrow.update(-1.0f, 0.0f, 1.0f);
  }

  // Perform

  // Asserts
  TEST_ASSERT_TRUE(closeEnough(
      wideThenNarrow.update(1.0f, 0.0f, 1.0f, -5.0f, -1.0f), -5.0f));
}

void test_bounded_pi_basic_clamping() {
  // Prepare
  resetMocks();
  BoundedPIController pi(1.0F, 1.0F, 0.5F, 1.0F);

  // Perform

  // Asserts
  TEST_ASSERT_FLOAT_WITHIN(0.0001F, 0.5F, pi.update(2.0F, 0.0F, 1.0F));
  TEST_ASSERT_FLOAT_WITHIN(0.0001F, 0.0F,
                           pi.update(1.0F, 0.0F, 1.0F, 1.0F, -1.0F));
}

void test_bounded_pi_reset_returns_to_zero() {
  // Prepare
  resetMocks();
  BoundedPIController pi(1.0F, 1.0F, 0.5F, 1.0F);
  pi.update(2.0F, 0.0F, 1.0F);
  pi.update(1.0F, 0.0F, 1.0F, 1.0F, -1.0F);

  // Perform
  pi.reset();

  // Asserts
  TEST_ASSERT_FLOAT_WITHIN(0.0001F, 0.0F, pi.update(0.0F, 0.0F, 1.0F));
}


int main() {
  UNITY_BEGIN();
  RUN_TEST(test_bounded_pi_basic_clamping);
  RUN_TEST(test_bounded_pi_reset_returns_to_zero);
  RUN_TEST(test_bounded_pi_clamps_positive_error_to_max_correction);
  RUN_TEST(test_bounded_pi_clamps_negative_error);
  RUN_TEST(test_bounded_pi_integral_step_size_matches);
  RUN_TEST(test_bounded_pi_custom_lower_bound_clamps_negative);
  RUN_TEST(test_bounded_pi_custom_upper_bound_clamps_positive);
  RUN_TEST(test_bounded_pi_holds_integral_when_saturated);
  RUN_TEST(test_bounded_pi_reset_clears_integral);
  RUN_TEST(test_bounded_pi_negative_ctor_limits_absolutized);
  RUN_TEST(test_bounded_pi_releases_saturation_with_negative_error);
  RUN_TEST(test_bounded_pi_allowed_drift_when_clamped_high);
  RUN_TEST(test_bounded_pi_integrates_negative_then_clamps_with_positive_error);
  return UNITY_END();
}
