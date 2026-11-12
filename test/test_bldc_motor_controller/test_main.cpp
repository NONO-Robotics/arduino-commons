#include <unity.h>

#include "NativeTestCommon.h"

class FixedSpeedToPwmConverter : public SpeedToPwmConverter {
 public:
  int convert(float) const override { return 42; }
};

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

void test_bldc_motor_controller_uses_an_injected_converter() {
  resetMocks();
  BLDCMotor* builtMotor = BLDCMotorBuilder(4, 5, 6)
                               .setResolutionInBits(8)->setChannel(2)
                               ->setFrequency(1000)->build();
  FixedSpeedToPwmConverter converter;
  BLDCMotorController controller(builtMotor, converter);

  controller.setup()->setRadsBySegSpeed(1.0F);

  TEST_ASSERT_EQUAL_INT(42, arduino_mock::pwm_values[4]);
}

void test_table_speed_to_pwm_converter_interpolates_and_clamps() {
  constexpr TableSpeedToPwmConverter::Point points[] = {
      {5, 0.9F},
      {10, 1.9F},
  };
  TableSpeedToPwmConverter converter(points, 2);

  TEST_ASSERT_EQUAL_INT(0, converter.convert(0.001F));
  TEST_ASSERT_EQUAL_INT(5, converter.convert(0.9F));
  TEST_ASSERT_EQUAL_INT(8, converter.convert(1.4F));
  TEST_ASSERT_EQUAL_INT(-10, converter.convert(-2.0F));
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_bldc_motor_controller_maps_rads_and_pwm);
  RUN_TEST(test_bldc_motor_controller_uses_an_injected_converter);
  RUN_TEST(test_table_speed_to_pwm_converter_interpolates_and_clamps);
  return UNITY_END();
}
