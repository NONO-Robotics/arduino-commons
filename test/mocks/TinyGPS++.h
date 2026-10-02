#pragma once

#include <Arduino.h>

namespace tinygps_mock {
struct LocationState {
  bool valid = false;
  bool updated = false;
  double latitude = 0;
  double longitude = 0;
};
inline bool location_scripted = false;
inline LocationState location;
inline void setLocation(double latitude, double longitude, bool valid, bool updated) {
  location = {valid, updated, latitude, longitude};
  location_scripted = true;
}
inline bool time_scripted = false;
inline uint8_t time_hour = 0;
inline uint8_t time_minute = 0;
inline uint8_t time_second = 0;
inline bool time_valid = false;
inline void setTime(uint8_t hour, uint8_t minute, uint8_t second, bool valid = true) {
  time_hour = hour;
  time_minute = minute;
  time_second = second;
  time_valid = valid;
  time_scripted = true;
}
inline bool hdop_scripted = false;
inline double hdop_value = 0;
inline bool hdop_valid = false;
inline void setHdop(double value, bool valid = true) {
  hdop_value = value;
  hdop_valid = valid;
  hdop_scripted = true;
}
inline void reset() {
  location = {};
  location_scripted = false;
  time_hour = time_minute = time_second = 0;
  time_valid = false;
  time_scripted = false;
  hdop_value = 0;
  hdop_valid = false;
  hdop_scripted = false;
}
}

class TinyGPSPlus {
 public:
  struct Location { bool valid = false; bool updated = false; bool isValid() const { return valid; } bool isUpdated() const { return updated; } double lat() const { return latitude; } double lng() const { return longitude; } double latitude = 0; double longitude = 0; } location;
  struct Date { bool valid = false; bool isValid() const { return valid; } uint16_t year() const { return value_year; } uint8_t month() const { return value_month; } uint8_t day() const { return value_day; } uint16_t value_year = 0; uint8_t value_month = 0; uint8_t value_day = 0; } date;
  struct Time { bool valid = false; bool isValid() const { return valid; } uint8_t hour() const { return value_hour; } uint8_t minute() const { return value_minute; } uint8_t second() const { return value_second; } uint8_t value_hour = 0; uint8_t value_minute = 0; uint8_t value_second = 0; } time;
  struct Integer { bool valid = false; bool isValid() const { return valid; } uint32_t value() const { return data; } uint32_t data = 0; } satellites;
  struct Decimal { bool valid = false; bool isValid() const { return valid; } double mps() const { return data; } double kmph() const { return data; } double meters() const { return data; } double value() const { return data; } double data = 0; } speed, altitude, hdop;
  bool encode(char value) {
    (void)value;
    if (tinygps_mock::location_scripted) {
      location.valid = tinygps_mock::location.valid;
      location.updated = tinygps_mock::location.updated;
      location.latitude = tinygps_mock::location.latitude;
      location.longitude = tinygps_mock::location.longitude;
    }
    if (tinygps_mock::time_scripted) {
      time.valid = tinygps_mock::time_valid;
      time.value_hour = tinygps_mock::time_hour;
      time.value_minute = tinygps_mock::time_minute;
      time.value_second = tinygps_mock::time_second;
    }
    if (tinygps_mock::hdop_scripted) {
      hdop.valid = tinygps_mock::hdop_valid;
      hdop.data = tinygps_mock::hdop_value;
    }
    return true;
  }
};
