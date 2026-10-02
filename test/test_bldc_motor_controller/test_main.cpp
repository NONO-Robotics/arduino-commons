#include <unity.h>

#include "NativeTestCommon.h"

void test_bldc_motor_controller_maps_rads_and_pwm() {
  // Prepare
  resetMocks();
  BLDCMotor* builtMotor = BLDCMotorBuilder(4, 5, 6)
                               .setResolutionInBits(8)->setChannel(2)
                               ->setFrequency(1000)->invertDirection()->build();
  BLDCMotorController directController(builtMotor, 4.0F, 10, 100);

  // Perform
  directController.setup()->setRadsBySegSpeed(2.0F);

  // Asserts
  TEST_ASSERT_GREATER_THAN(0, arduino_mock::pwm_values[4]);
  TEST_ASSERT_EQUAL_PTR(&directController, directController.setPwmSpeed(150));
  TEST_ASSERT_EQUAL_INT(150, arduino_mock::pwm_values[4]);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_bldc_motor_controller_maps_rads_and_pwm);
  return UNITY_END();
}
