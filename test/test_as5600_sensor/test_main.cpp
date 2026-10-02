#include <unity.h>

#include "NativeTestCommon.h"

void test_as5600_short_read_reports_error() {
  // Prepare
  resetMocks();
  Wire.reset();
  AS5600Sensor shortReadSensor(&Wire, AS5600_DEFAULT_ADDR);
  Wire.queue(AS5600_DEFAULT_ADDR, {0x12});

  // Perform

  // Asserts
  TEST_ASSERT_EQUAL_INT(AS5600_ERROR_VALUE, shortReadSensor.update());
  TEST_ASSERT_EQUAL_INT(1, Wire.requests_.size());
  TEST_ASSERT_EQUAL_INT(AS5600_DEFAULT_ADDR, Wire.requests_[0].address);
  TEST_ASSERT_EQUAL_INT(2, Wire.requests_[0].count);
  TEST_ASSERT_EQUAL_INT(0x12, Wire.read());
  TEST_ASSERT_EQUAL_INT(-1, Wire.read());
}

void test_as5600_sensors_use_their_own_addresses() {
  // Prepare
  resetMocks();
  AS5600Sensor first(&Wire, 0x36);
  AS5600Sensor second(&Wire, 0x37);
  Wire.queue(0x36, {0x00, 0x01});
  Wire.queue(0x37, {0x00, 0x02});

  // Perform

  // Asserts
  TEST_ASSERT_EQUAL_INT(1, first.update());
  TEST_ASSERT_EQUAL_INT(2, second.update());
  TEST_ASSERT_EQUAL_INT(2, Wire.requests_.size());
  TEST_ASSERT_EQUAL_INT(0x36, Wire.requests_[0].address);
  TEST_ASSERT_EQUAL_INT(0x37, Wire.requests_[1].address);
}

void test_as5600_end_transmission_failure_returns_error() {
  // Prepare
  resetMocks();
  AS5600Sensor failedTransaction(&Wire, 0x36);
  Wire.scriptEndTransmission(4);

  // Perform

  // Asserts
  TEST_ASSERT_EQUAL_INT(AS5600_ERROR_VALUE, failedTransaction.update());
  TEST_ASSERT_EQUAL_INT(0, Wire.requests_.size());
}

void test_as5600_short_request_returns_error() {
  // Prepare
  resetMocks();
  AS5600Sensor failedRequest(&Wire, 0x36);
  Wire.scriptRequestFrom(0);

  // Perform

  // Asserts
  TEST_ASSERT_EQUAL_INT(AS5600_ERROR_VALUE, failedRequest.update());
  TEST_ASSERT_EQUAL_INT(1, Wire.requests_.size());
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_as5600_short_read_reports_error);
  RUN_TEST(test_as5600_sensors_use_their_own_addresses);
  RUN_TEST(test_as5600_end_transmission_failure_returns_error);
  RUN_TEST(test_as5600_short_request_returns_error);
  return UNITY_END();
}
