#include <unity.h>

#include "NativeTestCommon.h"

void test_bldc_motor_forward_pwm_sets_direction_low() {
  // Prepare
  resetMocks();
  BLDCMotor motor(1, 2, 3, 8);

  // Perform
  motor.setup()->setPwmSpeed(42);

  // Asserts
  TEST_ASSERT_EQUAL_INT(42, arduino_mock::pwm_values[1]);
  TEST_ASSERT_EQUAL_INT(LOW, arduino_mock::digital_values[2]);
  TEST_ASSERT_EQUAL_INT(LOW, arduino_mock::digital_values[3]);
}

void test_bldc_motor_reverse_pwm_sets_direction_high() {
  // Prepare
  resetMocks();
  BLDCMotor motor(1, 2, 3, 8);

  // Perform
  motor.setup();
  motor.setPwmSpeed(-42);

  // Asserts
  TEST_ASSERT_EQUAL_INT(HIGH, arduino_mock::digital_values[2]);
}

void test_bldc_motor_stop_brakes_and_zeroes_pwm() {
  // Prepare
  resetMocks();
  BLDCMotor motor(1, 2, 3, 8);
  motor.setup();
  motor.setPwmSpeed(42);

  // Perform
  motor.stop();

  // Asserts
  TEST_ASSERT_EQUAL_INT(HIGH, arduino_mock::digital_values[3]);
  TEST_ASSERT_EQUAL_INT(0, arduino_mock::pwm_values[1]);
}

void test_bldc_motor_clamps_pwm_to_resolution() {
  // Prepare
  resetMocks();
  BLDCMotor motor(1, 2, 3, 8);

  // Perform
  motor.setup()->setPwmSpeed(300);

  // Asserts
  TEST_ASSERT_EQUAL_INT(255, arduino_mock::pwm_values[1]);
}

void test_bldc_motor_zero_pwm_clears_duty() {
  // Prepare
  resetMocks();
  BLDCMotor motor(1, 2, 3, 8);
  motor.setup();
  motor.setPwmSpeed(300);

  // Perform
  motor.setPwmSpeed(0);

  // Asserts
  TEST_ASSERT_EQUAL_INT(0, arduino_mock::pwm_values[1]);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_bldc_motor_forward_pwm_sets_direction_low);
  RUN_TEST(test_bldc_motor_reverse_pwm_sets_direction_high);
  RUN_TEST(test_bldc_motor_stop_brakes_and_zeroes_pwm);
  RUN_TEST(test_bldc_motor_clamps_pwm_to_resolution);
  RUN_TEST(test_bldc_motor_zero_pwm_clears_duty);
  return UNITY_END();
}
