#include <unity.h>

#include "NativeTestCommon.h"

void test_gps_data_defaults_when_no_fix() {
  // Prepare
  resetMocks();
  GPSData data;

  // Perform
  data.encode('x');

  // Asserts
  TEST_ASSERT_FALSE(data.isLocationUpdated());
  TEST_ASSERT_FALSE(data.isLocationValid());
  TEST_ASSERT_EQUAL_FLOAT(99.0F, data.getHDOP());
  TEST_ASSERT_EQUAL_INT(0, data.getTimeHour());
  TEST_ASSERT_EQUAL_INT(0, data.getTimeMinute());
  TEST_ASSERT_EQUAL_INT(0, data.getTimeSecond());
  TEST_ASSERT_EQUAL_INT(0, data.getSatellites());
  TEST_ASSERT_EQUAL_FLOAT(0.0F, data.getAltitude());
  TEST_ASSERT_EQUAL_FLOAT(0.0F, data.getLatitude());
  TEST_ASSERT_EQUAL_FLOAT(0.0F, data.getLongitude());
}

void test_gps_data_time_and_hdop_when_valid() {
  // Prepare
  resetMocks();
  GPSData data;
  tinygps_mock::setTime(13, 45, 30, true);
  tinygps_mock::setHdop(1.5, true);

  // Perform
  data.encode('x');

  // Asserts
  TEST_ASSERT_EQUAL_INT(13, data.getTimeHour());
  TEST_ASSERT_EQUAL_INT(45, data.getTimeMinute());
  TEST_ASSERT_EQUAL_INT(30, data.getTimeSecond());
  TEST_ASSERT_FLOAT_WITHIN(0.0001F, 1.5F, data.getHDOP());
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_gps_data_defaults_when_no_fix);
  RUN_TEST(test_gps_data_time_and_hdop_when_valid);
  return UNITY_END();
}
