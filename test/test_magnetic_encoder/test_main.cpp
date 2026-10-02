#include <unity.h>

#include "NativeTestCommon.h"

void test_magnetic_encoder_builder_defaults_fail_begin() {
  // Prepare
  resetMocks();
  MagneticEncoderBuilder defaults;

  // Perform
  MagneticEncoder* defaultEncoder = defaults.build();

  // Asserts
  TEST_ASSERT_FALSE(defaultEncoder->begin());
}

void test_magnetic_encoder_read_failure_keeps_speed_zero() {
  // Prepare
  resetMocks();
  Wire.reset();
  Wire.queue({0, 0});
  MagneticEncoder* encoder = MagneticEncoderBuilder()
                                .setCallback(onEncoderUpdate)
                                .setChannel(0)
                                .setSampleInterval(1)
                                .setI2CPort(&Wire)
                                .build();

  // Perform

  // Asserts
  TEST_ASSERT_TRUE(encoder->begin());
  arduino_mock::now_ms = 1;
  Wire.scriptEndTransmission(4);
  encoder->update();
  TEST_ASSERT_EQUAL_FLOAT(0.0F, sampledSpeeds.getFlWInRad());
}

void test_magnetic_encoder_skips_update_before_sample_interval() {
  // Prepare
  resetMocks();
  Wire.queue({0, 0});
  MagneticEncoder* encoder = MagneticEncoderBuilder()
                                .setCallback(onEncoderUpdate)
                                .setChannel(0)
                                .setSampleInterval(1)
                                .setI2CAddress(AS5600_DEFAULT_ADDR)
                                .setI2CPort(&Wire)
                                .build();

  // Perform

  // Asserts
  TEST_ASSERT_TRUE(encoder->begin());
  encoder->update();
  TEST_ASSERT_EQUAL_INT(0, encoder->getStep());
}

void test_magnetic_encoder_forward_wrap() {
  // Prepare
  resetMocks();
  Wire.queue({0, 0});
  MagneticEncoder* encoder = MagneticEncoderBuilder()
                                .setCallback(onEncoderUpdate)
                                .setChannel(0)
                                .setSampleInterval(1)
                                .setI2CAddress(AS5600_DEFAULT_ADDR)
                                .setI2CPort(&Wire)
                                .build();
  encoder->begin();

  // Perform
  arduino_mock::now_ms = 1;
  Wire.queue({0x0B, 0xB8});
  encoder->update();

  // Asserts
  TEST_ASSERT_EQUAL_INT(3000, encoder->getStep());
  TEST_ASSERT_TRUE(encoder->getW() < 0.0F);
}

void test_magnetic_encoder_reverse_wrap() {
  // Prepare
  resetMocks();
  Wire.queue({0, 0});
  MagneticEncoder* encoder = MagneticEncoderBuilder()
                                .setCallback(onEncoderUpdate)
                                .setChannel(0)
                                .setSampleInterval(1)
                                .setI2CAddress(AS5600_DEFAULT_ADDR)
                                .setI2CPort(&Wire)
                                .build();
  encoder->begin();
  arduino_mock::now_ms = 1;
  Wire.queue({0x0B, 0xB8});
  encoder->update();

  // Perform
  arduino_mock::now_ms = 2;
  Wire.queue({0, 0});
  encoder->update();

  // Asserts
  TEST_ASSERT_EQUAL_INT(0, encoder->getStep());
  TEST_ASSERT_TRUE(encoder->getW() > 0.0F);
}

void test_magnetic_encoder_builder_filter_flags_build() {
  // Prepare
  resetMocks();
  MagneticEncoderBuilder encoderBuilder;
  Wire.queue({0, 0});
  MagneticEncoder* encoder = encoderBuilder.setCallback(onEncoderUpdate)
                                .setChannel(5).setSampleInterval(1)
                                .setAlpha(1.0).withFilter(false).setDeadZone(0)
                                .setI2CPort(&Wire).build();

  // Perform

  // Asserts
  TEST_ASSERT_TRUE(encoder->begin());
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_magnetic_encoder_builder_defaults_fail_begin);
  RUN_TEST(test_magnetic_encoder_read_failure_keeps_speed_zero);
  RUN_TEST(test_magnetic_encoder_skips_update_before_sample_interval);
  RUN_TEST(test_magnetic_encoder_forward_wrap);
  RUN_TEST(test_magnetic_encoder_reverse_wrap);
  RUN_TEST(test_magnetic_encoder_builder_filter_flags_build);
  return UNITY_END();
}
