#include <unity.h>

#include "NativeTestCommon.h"

void test_encoder_angular_velocity_estimator_conversion() {
  // Prepare
  resetMocks();
  EncoderAngularVelocityEstimator estimator(1.0, 0.0F);

  // Perform

  // Asserts
  TEST_ASSERT_FLOAT_WITHIN(0.001F, 1.53398F,
                           estimator.getWInRadBySec(100, 100, false));
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_encoder_angular_velocity_estimator_conversion);
  return UNITY_END();
}
