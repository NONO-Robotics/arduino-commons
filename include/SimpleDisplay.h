#pragma once
#include <U8g2lib.h>

/**
 * @brief Wrapper class for OLED displays (SH1106/SSD1306).
 *
 * Simplified interface for writing text using U8g2.
 */
class SimpleDisplay {
private:
  U8G2_SH1106_128X64_NONAME_F_HW_I2C display;
  unsigned short currentY;
  unsigned short xOffset;
  unsigned short yOffset;

public:
  /**
   * @brief Construct a new Simple Display object.
   *
   * @param sdPin SDA pin number (not used directly if using HW I2C, but kept
   * for interface compatibility).
   * @param sclPin SCL pin number.
   * @param fontStyle U8g2 font to use.
   * @param xOffset X offset for text start.
   * @param yOffset Y offset for text start.
   */
  SimpleDisplay(int sdPin, int sclPin,
                const uint8_t *fontStyle = u8g2_font_5x8_tf,
                unsigned short xOffset = 2, unsigned short yOffset = 9);

  /**
   * @brief Write text to the display buffer.
   * @param value Text to write.
   * @return Pointer to SimpleDisplay for chaining.
   */
  SimpleDisplay *write(String value);

  /**
   * @brief Render the buffer to the screen.
   * @return Pointer to SimpleDisplay.
   */
  SimpleDisplay *render();

  /**
   * @brief Clear the display buffer.
   * @return Pointer to SimpleDisplay.
   */
  SimpleDisplay *clean();
};