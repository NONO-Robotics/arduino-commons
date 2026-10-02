#include <unity.h>

#include "NativeTestCommon.h"

void test_four_wheel_controller_applies_speed_and_directions() {
  // Prepare
  resetMocks();
  FourWheelBLDCController controller(settings(11, 12, 13, 0, true),
                                     settings(14, 15, 16, 1),
                                     settings(17, 18, 19, 2, true),
                                     settings(20, 21, 22, 3));
  FourWheelAngularSpeed command;
  command.updateFrom(2.0F, -2.0F, -1.0F, 1.0F);

  // Perform
  controller.applySpeed(command);

  // Asserts
  TEST_ASSERT_GREATER_THAN(0, arduino_mock::pwm_values[11]);
  TEST_ASSERT_GREATER_THAN(0, arduino_mock::pwm_values[14]);
  TEST_ASSERT_EQUAL_INT(LOW, arduino_mock::digital_values[12]);
  TEST_ASSERT_EQUAL_INT(LOW, arduino_mock::digital_values[15]);
  TEST_ASSERT_EQUAL_INT(HIGH, arduino_mock::digital_values[18]);
  TEST_ASSERT_EQUAL_INT(HIGH, arduino_mock::digital_values[21]);
  TEST_ASSERT_EQUAL_INT(OUTPUT, arduino_mock::pin_modes[11]);
  TEST_ASSERT_EQUAL_INT(OUTPUT, arduino_mock::pin_modes[14]);
  TEST_ASSERT_EQUAL_INT(OUTPUT, arduino_mock::pin_modes[17]);
  TEST_ASSERT_EQUAL_INT(OUTPUT, arduino_mock::pin_modes[20]);
}

void test_four_wheel_controller_stop_engages_all_brakes() {
  // Prepare
  resetMocks();
  FourWheelBLDCController controller(settings(11, 12, 13, 0, true),
                                     settings(14, 15, 16, 1),
                                     settings(17, 18, 19, 2, true),
                                     settings(20, 21, 22, 3));
  FourWheelAngularSpeed command;
  command.updateFrom(2.0F, -2.0F, -1.0F, 1.0F);
  controller.applySpeed(command);

  // Perform
  controller.stop();

  // Asserts
  TEST_ASSERT_EQUAL_INT(HIGH, arduino_mock::digital_values[13]);
  TEST_ASSERT_EQUAL_INT(HIGH, arduino_mock::digital_values[16]);
  TEST_ASSERT_EQUAL_INT(HIGH, arduino_mock::digital_values[19]);
  TEST_ASSERT_EQUAL_INT(HIGH, arduino_mock::digital_values[22]);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_four_wheel_controller_applies_speed_and_directions);
  RUN_TEST(test_four_wheel_controller_stop_engages_all_brakes);
  return UNITY_END();
}
