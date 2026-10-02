#pragma once

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <map>
#include <sstream>
#include <string>
#include <type_traits>

using byte = uint8_t;
using boolean = bool;
class String : public std::string {
 public:
  using std::string::string;
  String(const std::string& value) : std::string(value) {}
  template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
  String(T value) : std::string(std::to_string(value)) {}
  long toInt() const { return std::strtol(c_str(), nullptr, 10); }
  operator const char*() const { return c_str(); }
};

constexpr uint8_t LOW = 0;
constexpr uint8_t HIGH = 1;
constexpr uint8_t INPUT = 0;
constexpr uint8_t OUTPUT = 1;
constexpr uint8_t INPUT_PULLUP = 2;
constexpr uint32_t SERIAL_8N1 = 0;
enum esp_reset_reason_t { ESP_RST_POWERON, ESP_RST_SW, ESP_RST_UNKNOWN };

namespace arduino_mock {
  inline unsigned long now_ms = 0;
  inline esp_reset_reason_t reset_reason = ESP_RST_POWERON;
inline std::map<uint8_t, int> pin_modes;
inline std::map<uint8_t, int> digital_values;
inline std::map<uint8_t, int> analog_values;
inline std::map<uint8_t, int> pwm_values;
struct PwmSetup { uint32_t frequency; uint8_t resolution; };
inline std::map<uint8_t, PwmSetup> pwm_setups;
inline std::map<uint8_t, uint8_t> pwm_pins;

inline void reset() {
  now_ms = 0;
  pin_modes.clear();
  digital_values.clear();
  analog_values.clear();
  pwm_values.clear();
  pwm_setups.clear();
  pwm_pins.clear();
  reset_reason = ESP_RST_POWERON;
}
}

inline unsigned long millis() { return arduino_mock::now_ms; }
inline unsigned long micros() { return arduino_mock::now_ms * 1000UL; }
inline void delay(unsigned long duration) { arduino_mock::now_ms += duration; }
inline void pinMode(uint8_t pin, uint8_t mode) { arduino_mock::pin_modes[pin] = mode; }
inline int digitalRead(uint8_t pin) { return arduino_mock::digital_values[pin]; }
inline void digitalWrite(uint8_t pin, uint8_t value) { arduino_mock::digital_values[pin] = value; }
inline int analogRead(uint8_t pin) { return arduino_mock::analog_values[pin]; }
inline void analogWrite(uint8_t pin, int value) { arduino_mock::pwm_values[pin] = value; }
template <typename T> inline T constrain(T value, T low, T high) { return std::clamp(value, low, high); }
inline void ledcAttach(uint8_t pin, uint32_t frequency, uint8_t resolution) {
  arduino_mock::pin_modes[pin] = OUTPUT;
  arduino_mock::pwm_setups[pin] = {frequency, resolution};
}
inline void ledcWrite(uint8_t channel, int value) { arduino_mock::pwm_values[channel] = value; }
inline double ledcSetup(uint8_t channel, double frequency, uint8_t resolution) {
  arduino_mock::pwm_setups[channel] = {static_cast<uint32_t>(frequency), resolution};
  return frequency;
}
inline void ledcAttachPin(uint8_t pin, uint8_t channel) {
  arduino_mock::pin_modes[pin] = OUTPUT;
  arduino_mock::pwm_pins[channel] = pin;
}
inline long random(long max) { return max ? 0 : 0; }
inline long random(long min, long max) { return min < max ? min : max; }
template <typename... Servers> inline void configTime(long, int, Servers...) {}

class HardwareSerial {
 public:
  void begin(unsigned long, uint32_t = SERIAL_8N1, int = -1, int = -1) {}
  int available() const { return static_cast<int>(input_.size()); }
  int read() {
    if (input_.empty()) return -1;
    const char value = input_.front();
    input_.erase(0, 1);
    return static_cast<unsigned char>(value);
  }
  size_t print(const String& value) { output_ += value; return value.size(); }
  size_t println(const String& value = "") { output_ += value + '\n'; return value.size() + 1; }
  void inject(const String& value) { input_ += value; }
  const String& output() const { return output_; }
  void clear() { input_.clear(); output_.clear(); }

 private:
  String input_;
  String output_;
};

inline HardwareSerial Serial;

inline esp_reset_reason_t esp_reset_reason() { return arduino_mock::reset_reason; }
