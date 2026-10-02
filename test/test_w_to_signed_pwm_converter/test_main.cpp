#include <unity.h>

#include "NativeTestCommon.h"

void test_w_to_signed_pwm_converter_mapping() {
  // Prepare
  resetMocks();
  WToSignedPWMConverter converter(4.0F, 8, 10, 100);

  // Perform

  // Asserts
  TEST_ASSERT_EQUAL_INT(0, converter.convert(0.0F));
  TEST_ASSERT_EQUAL_INT(0, converter.convert(0.001F));
  TEST_ASSERT_EQUAL_INT(10, converter.convert(0.002F));
  TEST_ASSERT_EQUAL_INT(100, converter.convert(8.0F));
  TEST_ASSERT_EQUAL_INT(100, converter.convert(20.0F));
  TEST_ASSERT_EQUAL_INT(-100, converter.convert(-8.0F));
}

void test_w_to_signed_pwm_converter_full_resolution_when_max_pwm_disabled() {
  // Prepare
  resetMocks();
  WToSignedPWMConverter limitConverter(4.0F, 8, 10, 0);

  // Perform

  // Asserts
  TEST_ASSERT_EQUAL_INT(255, limitConverter.convert(20.0F));
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_w_to_signed_pwm_converter_mapping);
  RUN_TEST(test_w_to_signed_pwm_converter_full_resolution_when_max_pwm_disabled);
  return UNITY_END();
}
