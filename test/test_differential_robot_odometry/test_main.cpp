#include <unity.h>

#include "NativeTestCommon.h"

void test_differential_odometry_left_speed() {
  // Prepare
  resetMocks();
  FourWheelAngularSpeed speeds;
  speeds.updateFrom(1.0F, 1.0F, 1.0F, 1.0F);
  speeds.setBrWInRad(2.0F);
  DifferentialRobotOdometry odometry;

  // Perform
  odometry.updateFrom(speeds);

  // Asserts
  TEST_ASSERT_FLOAT_WITHIN(0.0001F, 1.0F, odometry.getLeftWInRad());
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_differential_odometry_left_speed);
  return UNITY_END();
}
