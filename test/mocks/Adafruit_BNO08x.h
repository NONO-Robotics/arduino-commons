#pragma once

#include <Arduino.h>
#include <Wire.h>

#include <deque>
#include <map>
#include <vector>

constexpr uint8_t SH2_ROTATION_VECTOR = 0;
constexpr uint8_t SH2_GAME_ROTATION_VECTOR = 1;
constexpr uint8_t SH2_GYROSCOPE_CALIBRATED = 2;
constexpr uint8_t SH2_LINEAR_ACCELERATION = 3;

struct sh2_RotationVectorWAcc_t { float real = 0; float i = 0; float j = 0; float k = 0; float accuracy = 0; };
struct sh2_Gyroscope_t { float x = 0; float y = 0; float z = 0; };
struct sh2_LinearAcceleration_t { float x = 0; float y = 0; float z = 0; };
struct sh2_SensorValue_t { uint8_t sensorId = 0; union { sh2_RotationVectorWAcc_t rotationVector; sh2_Gyroscope_t gyroscope; sh2_LinearAcceleration_t linearAcceleration; } un;
  sh2_SensorValue_t() : un{} {} };

namespace imu_mock {
inline bool begin_result = true;
inline bool reset = false;
inline std::deque<bool> begin_results;
inline std::map<uint8_t, std::deque<bool>> report_results;
inline std::deque<sh2_SensorValue_t> events;
inline std::vector<uint8_t> enabled_reports;
inline void resetState() {
  begin_result = true;
  reset = false;
  begin_results.clear();
  report_results.clear();
  events.clear();
  enabled_reports.clear();
}
inline void scriptBegin(bool result) { begin_results.push_back(result); }
inline void scriptReport(uint8_t report, bool result) {
  report_results[report].push_back(result);
}
inline void queueEvent(const sh2_SensorValue_t& event) { events.push_back(event); }
}

class Adafruit_BNO08x {
 public:
  explicit Adafruit_BNO08x(int = -1) {}
  bool begin_I2C(uint8_t = 0x4A, TwoWire* = nullptr) {
    if (!imu_mock::begin_results.empty()) {
      const bool result = imu_mock::begin_results.front();
      imu_mock::begin_results.pop_front();
      return result;
    }
    return imu_mock::begin_result;
  }
  bool enableReport(uint8_t report, long = 0) {
    imu_mock::enabled_reports.push_back(report);
    auto& results = imu_mock::report_results[report];
    if (results.empty()) return true;
    const bool result = results.front();
    results.pop_front();
    return result;
  }
  bool wasReset() {
    const bool was_reset = imu_mock::reset;
    imu_mock::reset = false;
    return was_reset;
  }
  bool getSensorEvent(sh2_SensorValue_t* value) {
    if (imu_mock::events.empty()) return false;
    *value = imu_mock::events.front();
    imu_mock::events.pop_front();
    return true;
  }
};
