#include <unity.h>

#include "NativeTestCommon.h"

void test_imu_init_fails_when_begin_fails() {
  // Prepare
  resetMocks();
  imu_mock::begin_result = false;
  IMUSensor beginFailure(onImuUpdate);

  // Perform

  // Asserts
  TEST_ASSERT_FALSE(beginFailure.init());
}

void test_imu_init_fails_when_rotation_report_rejected() {
  // Prepare
  resetMocks();
  imu_mock::scriptReport(SH2_GAME_ROTATION_VECTOR, false);
  IMUSensor reportFailure(onImuUpdate);

  // Perform

  // Asserts
  TEST_ASSERT_FALSE(reportFailure.init());
}

void test_imu_rotation_event_updates_orientation() {
  // Prepare
  resetMocks();
  IMUSensor sensor(onImuUpdate);
  sh2_SensorValue_t rotation;
  rotation.sensorId = SH2_GAME_ROTATION_VECTOR;
  rotation.un.rotationVector = {1.0F, 2.0F, 3.0F, 4.0F};

  // Perform

  // Asserts
  TEST_ASSERT_TRUE(sensor.init());
  imu_mock::queueEvent(rotation);
  TEST_ASSERT_TRUE(sensor.update());
  TEST_ASSERT_EQUAL_INT(1, imuUpdates);
  TEST_ASSERT_FLOAT_WITHIN(0.0001F, 1.0F, latestImu->getOrientationW());
  TEST_ASSERT_FLOAT_WITHIN(0.0001F, 4.0F, latestImu->getOrientationZ());
}

void test_imu_gyro_event_updates_angular_velocity() {
  // Prepare
  resetMocks();
  IMUSensor sensor(onImuUpdate);
  sh2_SensorValue_t gyro;
  gyro.sensorId = SH2_GYROSCOPE_CALIBRATED;
  gyro.un.gyroscope = {5.0F, 6.0F, 7.0F};

  // Perform

  // Asserts
  TEST_ASSERT_TRUE(sensor.init());
  imu_mock::queueEvent(gyro);
  TEST_ASSERT_TRUE(sensor.update());
  TEST_ASSERT_EQUAL_INT(1, imuUpdates);
  TEST_ASSERT_FLOAT_WITHIN(0.0001F, 7.0F, latestImu->getAngularVelocityZ());
}

void test_imu_linear_acceleration_event_updates_values() {
  // Prepare
  resetMocks();
  IMUSensor sensor(onImuUpdate);
  sh2_SensorValue_t acceleration;
  acceleration.sensorId = SH2_LINEAR_ACCELERATION;
  acceleration.un.linearAcceleration = {8.0F, 9.0F, 10.0F};

  // Perform

  // Asserts
  TEST_ASSERT_TRUE(sensor.init());
  imu_mock::queueEvent(acceleration);
  TEST_ASSERT_TRUE(sensor.update());
  TEST_ASSERT_EQUAL_INT(1, imuUpdates);
  TEST_ASSERT_FLOAT_WITHIN(0.0001F, 8.0F, latestImu->getLinearAccelerationX());
  TEST_ASSERT_FLOAT_WITHIN(0.0001F, 9.0F, latestImu->getLinearAccelerationY());
  TEST_ASSERT_FLOAT_WITHIN(0.0001F, 10.0F,
                           latestImu->getLinearAccelerationZ());
}

void test_imu_update_returns_false_without_event() {
  // Prepare
  resetMocks();
  IMUSensor sensor(onImuUpdate);

  // Perform

  // Asserts
  TEST_ASSERT_TRUE(sensor.init());
  TEST_ASSERT_FALSE(sensor.update());
}

void test_imu_unknown_sensor_event_still_triggers_callback() {
  // Prepare
  resetMocks();
  IMUSensor sensor(onImuUpdate);
  sh2_SensorValue_t unknown;
  unknown.sensorId = 99;

  // Perform

  // Asserts
  TEST_ASSERT_TRUE(sensor.init());
  imu_mock::queueEvent(unknown);
  TEST_ASSERT_TRUE(sensor.update());
  TEST_ASSERT_EQUAL_INT(1, imuUpdates);
}

void test_imu_begin_retries_until_sensor_responds() {
  // Prepare
  resetMocks();
  imu_mock::scriptBegin(false);
  imu_mock::scriptBegin(true);
  IMUSensor retry(onImuUpdate);

  // Perform

  // Asserts
  TEST_ASSERT_EQUAL_PTR(&retry, retry.begin());
}

void test_imu_init_fails_when_linear_report_rejected() {
  // Prepare
  resetMocks();
  imu_mock::scriptReport(SH2_LINEAR_ACCELERATION, false);
  IMUSensor linearFailure(onImuUpdate);

  // Perform

  // Asserts
  TEST_ASSERT_FALSE(linearFailure.init());
}

void test_imu_init_fails_when_gyro_report_rejected() {
  // Prepare
  resetMocks();
  imu_mock::scriptReport(SH2_GYROSCOPE_CALIBRATED, false);
  IMUSensor gyroFailure(onImuUpdate);

  // Perform

  // Asserts
  TEST_ASSERT_FALSE(gyroFailure.init());
}

void test_imu_skips_linear_acceleration_when_interval_zero() {
  // Prepare
  resetMocks();
  IMUSensor noLinearAcceleration(onImuUpdate, &Wire,
                                 IMU_SENSOR_I2C_ADDRESS,
                                 IMU_SENSOR_ROTATION_VECTOR_INTERVAL_US, 0,
                                 IMU_SENSOR_GYROSCOPE_INTERVAL_US);

  // Perform

  // Asserts
  TEST_ASSERT_TRUE(noLinearAcceleration.init());
  TEST_ASSERT_FALSE(std::find(imu_mock::enabled_reports.begin(),
                              imu_mock::enabled_reports.end(),
                              SH2_LINEAR_ACCELERATION) !=
                    imu_mock::enabled_reports.end());
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_imu_init_fails_when_begin_fails);
  RUN_TEST(test_imu_init_fails_when_rotation_report_rejected);
  RUN_TEST(test_imu_rotation_event_updates_orientation);
  RUN_TEST(test_imu_gyro_event_updates_angular_velocity);
  RUN_TEST(test_imu_linear_acceleration_event_updates_values);
  RUN_TEST(test_imu_update_returns_false_without_event);
  RUN_TEST(test_imu_unknown_sensor_event_still_triggers_callback);
  RUN_TEST(test_imu_begin_retries_until_sensor_responds);
  RUN_TEST(test_imu_init_fails_when_linear_report_rejected);
  RUN_TEST(test_imu_init_fails_when_gyro_report_rejected);
  RUN_TEST(test_imu_skips_linear_acceleration_when_interval_zero);
  return UNITY_END();
}
