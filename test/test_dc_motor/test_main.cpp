#include <unity.h>

#include "NativeTestCommon.h"

void test_dc_motor_setup_configures_pins() {
  // Prepare
  resetMocks();
  DCMotor dcMotor(8, 9, 10);

  // Perform
  dcMotor.setup();

  // Asserts
  TEST_ASSERT_EQUAL_INT(OUTPUT, arduino_mock::pin_modes[8]);
  TEST_ASSERT_EQUAL_INT(OUTPUT, arduino_mock::pin_modes[9]);
  TEST_ASSERT_EQUAL_INT(OUTPUT, arduino_mock::pin_modes[10]);
}

void test_dc_motor_move_forward() {
  // Prepare
  resetMocks();
  DCMotor dcMotor(8, 9, 10);
  dcMotor.setup();

  // Perform
  dcMotor.move(50);

  // Asserts
  TEST_ASSERT_EQUAL_INT(HIGH, arduino_mock::digital_values[8]);
  TEST_ASSERT_EQUAL_INT(LOW, arduino_mock::digital_values[9]);
  TEST_ASSERT_EQUAL_INT(50, arduino_mock::pwm_values[10]);
}

void test_dc_motor_move_reverse() {
  // Prepare
  resetMocks();
  DCMotor dcMotor(8, 9, 10);
  dcMotor.setup();

  // Perform
  dcMotor.move(-30);

  // Asserts
  TEST_ASSERT_EQUAL_INT(LOW, arduino_mock::digital_values[8]);
  TEST_ASSERT_EQUAL_INT(HIGH, arduino_mock::digital_values[9]);
  TEST_ASSERT_EQUAL_INT(30, arduino_mock::pwm_values[10]);
}

void test_dc_motor_move_zero_stops_pwm() {
  // Prepare
  resetMocks();
  DCMotor dcMotor(8, 9, 10);
  dcMotor.setup();

  // Perform
  dcMotor.move(0);

  // Asserts
  TEST_ASSERT_EQUAL_INT(0, arduino_mock::pwm_values[10]);
}

void test_dc_motor_stop_brakes_and_clears_pwm() {
  // Prepare
  resetMocks();
  DCMotor dcMotor(8, 9, 10);
  dcMotor.setup();
  dcMotor.move(0);

  // Perform
  dcMotor.stop();

  // Asserts
  TEST_ASSERT_EQUAL_INT(HIGH, arduino_mock::digital_values[8]);
  TEST_ASSERT_EQUAL_INT(LOW, arduino_mock::digital_values[9]);
  TEST_ASSERT_EQUAL_INT(0, arduino_mock::pwm_values[10]);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_dc_motor_setup_configures_pins);
  RUN_TEST(test_dc_motor_move_forward);
  RUN_TEST(test_dc_motor_move_reverse);
  RUN_TEST(test_dc_motor_move_zero_stops_pwm);
  RUN_TEST(test_dc_motor_stop_brakes_and_clears_pwm);
  return UNITY_END();
}
