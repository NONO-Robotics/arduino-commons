#include <unity.h>

#include "ArduinoCommons.h"
#include "NativeTestCommon.h"

void test_wheel_speeds_mapper_preserves_diagnostic_order() {
  FourWheelAngularSpeed angularSpeed;
  angularSpeed.updateFrom(1.0F, 2.0F, 7.0F, 10.0F);
  DifferentialRobotOdometry robotOdometry;
  robotOdometry.updateFrom(angularSpeed);
  float wheelSpeeds[WheelSpeedsMapper::Length]{};

  WheelSpeedsMapper::toArray(wheelSpeeds, robotOdometry, angularSpeed);

  TEST_ASSERT_FLOAT_WITHIN(0.0001F, 4.0F, wheelSpeeds[0]);
  TEST_ASSERT_FLOAT_WITHIN(0.0001F, 6.0F, wheelSpeeds[1]);
  TEST_ASSERT_FLOAT_WITHIN(0.0001F, 1.0F, wheelSpeeds[2]);
  TEST_ASSERT_FLOAT_WITHIN(0.0001F, 2.0F, wheelSpeeds[3]);
  TEST_ASSERT_FLOAT_WITHIN(0.0001F, 7.0F, wheelSpeeds[4]);
  TEST_ASSERT_FLOAT_WITHIN(0.0001F, 10.0F, wheelSpeeds[5]);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_wheel_speeds_mapper_preserves_diagnostic_order);
  return UNITY_END();
}
