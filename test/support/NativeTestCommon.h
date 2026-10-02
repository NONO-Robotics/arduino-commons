#ifndef NATIVE_TEST_COMMON_H
#define NATIVE_TEST_COMMON_H

#include <Arduino.h>

#include "ArduinoCommons.h"
#include "BLDCMotorController.h"
#include "BoundedPIController.h"
#include "FourWheelBLDCController.h"
#include "FourWheelBLDCFeedbackController.h"
#include "I2CMultiplexor.h"
#include "MagneticEncoderUpdateServiceBuilder.h"
#include "StringUtils.h"
#include "timestamp.h"

namespace {
FourWheelAngularSpeed sampledSpeeds;
int gpsUpdates = 0;
int imuUpdates = 0;
int timerUpdates = 0;
GPSData* latestGps = nullptr;
IMUData* latestImu = nullptr;

void onEncoderUpdate(short int channel, int, float speed) {
  if (channel == 0) sampledSpeeds.setFlWInRad(speed);
  if (channel == 1) sampledSpeeds.setFrWInRad(speed);
  if (channel == 2) sampledSpeeds.setBlWInRad(speed);
  if (channel == 3) sampledSpeeds.setBrWInRad(speed);
}
void onGpsUpdate(GPSData* value) { latestGps = value; ++gpsUpdates; }
void onImuUpdate(IMUData* value) { latestImu = value; ++imuUpdates; }
void onTimer() { ++timerUpdates; }

void resetMocks() {
  arduino_mock::reset();
  Wire.reset();
  LittleFS.reset();
  arduinojson_mock::reset();
  tinygps_mock::reset();
  imu_mock::resetState();
  u8g2_mock::reset();
  logger_mock::debug_enabled = false;
  sampledSpeeds = FourWheelAngularSpeed();
  gpsUpdates = 0;
  imuUpdates = 0;
  timerUpdates = 0;
  latestGps = nullptr;
  latestImu = nullptr;
}

BLDCMotorSettings settings(int pwm, int direction, int brake, int channel,
                           bool inverted = false) {
  return {pwm, direction, brake, channel, inverted, 4.0F, 10, 100, 1.0F};
}
}

#endif  // NATIVE_TEST_COMMON_H
