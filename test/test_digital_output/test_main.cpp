#include <unity.h>

#include "NativeTestCommon.h"

void test_digital_output_initializes_low() {
  resetMocks();

  DigitalOutput output(7);

  TEST_ASSERT_EQUAL_INT(OUTPUT, arduino_mock::pin_modes[7]);
  TEST_ASSERT_EQUAL_INT(LOW, arduino_mock::digital_values[7]);
}

void test_digital_output_sets_high_and_low() {
  resetMocks();
  DigitalOutput output(7);

  output.high();
  TEST_ASSERT_EQUAL_INT(HIGH, arduino_mock::digital_values[7]);

  output.low();
  TEST_ASSERT_EQUAL_INT(LOW, arduino_mock::digital_values[7]);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_digital_output_initializes_low);
  RUN_TEST(test_digital_output_sets_high_and_low);
  return UNITY_END();
}
