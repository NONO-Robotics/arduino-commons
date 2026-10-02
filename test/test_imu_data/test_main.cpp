#include <unity.h>

#include "NativeTestCommon.h"

void test_imu_data_getters() {
  // Prepare
  resetMocks();
  IMUData imuData;
  imuData.setOrientation(1, 2, 3, 4);
  imuData.setAngularVelocity(5, 6, 7);
  imuData.setLinearAcceleration(8, 9, 10);

  // Perform

  // Asserts
  TEST_ASSERT_EQUAL_FLOAT(1, imuData.getOrientationW());
  TEST_ASSERT_EQUAL_FLOAT(2, imuData.getOrientationX());
  TEST_ASSERT_EQUAL_FLOAT(3, imuData.getOrientationY());
  TEST_ASSERT_EQUAL_FLOAT(4, imuData.getOrientationZ());
  TEST_ASSERT_EQUAL_FLOAT(5, imuData.getAngularVelocityX());
  TEST_ASSERT_EQUAL_FLOAT(6, imuData.getAngularVelocityY());
  TEST_ASSERT_EQUAL_FLOAT(7, imuData.getAngularVelocityZ());
  TEST_ASSERT_EQUAL_FLOAT(8, imuData.getLinearAccelerationX());
  TEST_ASSERT_EQUAL_FLOAT(9, imuData.getLinearAccelerationY());
  TEST_ASSERT_EQUAL_FLOAT(10, imuData.getLinearAccelerationZ());
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_imu_data_getters);
  return UNITY_END();
}
