#include <unity.h>

#include "NativeTestCommon.h"

void test_encoder_update_service_feeds_odometry() {
  // Prepare
  resetMocks();
  MagneticEncoderUpdateServiceBuilder builder(4);
  for (short channel = 0; channel < 4; ++channel) {
    Wire.queue({0, 0});
    builder.addEncoder(onEncoderUpdate, channel, 100, 1.0, false, 0.0F,
                       AS5600_DEFAULT_ADDR, &Wire);
  }
  MagneticEncoderUpdateService* service = builder.build();
  service->begin();
  arduino_mock::now_ms = 100;
  for (int index = 0; index < 4; ++index) Wire.queue({0, 100});

  // Perform
  service->update();

  // Asserts
  TEST_ASSERT_EQUAL_INT(4, builder.getSize());
  TEST_ASSERT_GREATER_THAN_FLOAT(0.0F, sampledSpeeds.getFlWInRad());
  TEST_ASSERT_GREATER_THAN_FLOAT(0.0F, sampledSpeeds.getFrWInRad());
  DifferentialRobotOdometry odometry;
  odometry.updateFrom(sampledSpeeds);
  TEST_ASSERT_FLOAT_WITHIN(0.001F, sampledSpeeds.getFlWInRad(),
                           odometry.getLeftWInRad());
  TEST_ASSERT_GREATER_THAN(4, static_cast<int>(Wire.transmissions_.size()));
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_encoder_update_service_feeds_odometry);
  return UNITY_END();
}
