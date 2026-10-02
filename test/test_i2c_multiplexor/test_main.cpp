#include <unity.h>

#include "NativeTestCommon.h"

void test_i2c_multiplexor_selects_valid_channel() {
  // Prepare
  resetMocks();
  I2CMultiplexor multiplexor(0x71);

  // Perform
  multiplexor.selectChannel(3);

  // Asserts
  TEST_ASSERT_EQUAL_INT(1, Wire.transmissions_.size());
  TEST_ASSERT_EQUAL_INT(0x71, Wire.transmissions_[0].address);
  TEST_ASSERT_EQUAL_INT(1, Wire.transmissions_[0].bytes.size());
  TEST_ASSERT_EQUAL_INT(0x08, Wire.transmissions_[0].bytes[0]);
}

void test_i2c_multiplexor_ignores_invalid_channel() {
  // Prepare
  resetMocks();
  I2CMultiplexor multiplexor(0x71);
  multiplexor.selectChannel(3);

  // Perform
  multiplexor.selectChannel(8);

  // Asserts
  TEST_ASSERT_EQUAL_INT(1, Wire.transmissions_.size());
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_i2c_multiplexor_selects_valid_channel);
  RUN_TEST(test_i2c_multiplexor_ignores_invalid_channel);
  return UNITY_END();
}
