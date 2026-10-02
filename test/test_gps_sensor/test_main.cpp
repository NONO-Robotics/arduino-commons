#include <unity.h>

#include "NativeTestCommon.h"

void test_gps_no_callback_when_location_not_updated() {
  // Prepare
  resetMocks();
  HardwareSerial serial;
  GPSSensor* sensor = GPSSensorBuilder(&serial).setPins(1, 2)
                          ->setOnUpdateEvent(onGpsUpdate)->setBaudRate(9600)->build();
  tinygps_mock::setLocation(-34.6, -58.4, false, false);

  // Perform
  serial.inject("x");
  sensor->update();

  // Asserts
  TEST_ASSERT_EQUAL_INT(0, gpsUpdates);
}

void test_gps_callback_delivers_valid_location() {
  // Prepare
  resetMocks();
  HardwareSerial serial;
  GPSSensor* sensor = GPSSensorBuilder(&serial).setPins(1, 2)
                          ->setOnUpdateEvent(onGpsUpdate)->setBaudRate(9600)->build();
  tinygps_mock::setLocation(-34.6037, -58.3816, true, true);

  // Perform
  serial.inject("x");
  sensor->update();

  // Asserts
  TEST_ASSERT_EQUAL_INT(1, gpsUpdates);
  TEST_ASSERT_NOT_NULL(latestGps);
  TEST_ASSERT_TRUE(latestGps->isLocationValid());
  TEST_ASSERT_FLOAT_WITHIN(0.0001F, -34.6037F, latestGps->getLatitude());
  TEST_ASSERT_FLOAT_WITHIN(0.0001F, -58.3816F, latestGps->getLongitude());
}

void test_gps_debug_logs_line_with_updates() {
  // Prepare
  resetMocks();
  HardwareSerial serial;
  GPSSensor* sensor = GPSSensorBuilder(&serial).setPins(1, 2)
                          ->setOnUpdateEvent(onGpsUpdate)->setBaudRate(9600)->build();
  logger_mock::debug_enabled = true;
  tinygps_mock::setLocation(-34.6, -58.4, true, true);

  // Perform
  serial.inject("abc");
  sensor->update();

  // Asserts
  TEST_ASSERT_EQUAL_INT(1, gpsUpdates);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_gps_no_callback_when_location_not_updated);
  RUN_TEST(test_gps_callback_delivers_valid_location);
  RUN_TEST(test_gps_debug_logs_line_with_updates);
  return UNITY_END();
}
