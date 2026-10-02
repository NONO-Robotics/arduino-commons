#pragma once

#include <Arduino.h>

namespace u8g2_mock {
inline int clear_calls = 0;
inline int send_calls = 0;
inline int draw_calls = 0;
inline int last_x = 0;
inline int last_y = 0;
inline String last_text;

inline void reset() {
  clear_calls = 0;
  send_calls = 0;
  draw_calls = 0;
  last_x = 0;
  last_y = 0;
  last_text.clear();
}
}

class U8G2 {
 public:
  void begin() {}
  void clearBuffer() { ++u8g2_mock::clear_calls; }
  void sendBuffer() { ++u8g2_mock::send_calls; }
  void setFont(const uint8_t*) {}
  void drawStr(int x, int y, const char* value) {
    ++u8g2_mock::draw_calls;
    u8g2_mock::last_x = x;
    u8g2_mock::last_y = y;
    u8g2_mock::last_text = value;
  }
};

constexpr uint8_t U8G2_R0 = 0;
constexpr uint8_t U8X8_PIN_NONE = 0;

class U8G2_SH1106_128X64_NONAME_F_HW_I2C : public U8G2 {
 public:
  U8G2_SH1106_128X64_NONAME_F_HW_I2C(uint8_t = 0, uint8_t = 0, uint8_t = 0, uint8_t = 0) {}
};

inline const uint8_t u8g2_font_ncenB08_tr[] = {};
inline const uint8_t u8g2_font_5x8_tf[] = {};
