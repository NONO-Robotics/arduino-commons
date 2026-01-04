#pragma once
#include <Arduino.h>

/**
 * @brief Simple debounced button class.
 */
class Button {
private:
  uint8_t pin;
  bool previousState;
  unsigned long lastDebounceTime = 0;
  const unsigned long debounceTime = 50; ///< Debounce time in milliseconds

public:
  /**
   * @brief Construct a new Button object.
   * @param p GPIO pin number.
   */
  Button(uint8_t p);

  /**
   * @brief Initialize button pin (INPUT_PULLUP by default).
   */
  void init();

  /**
   * @brief Check if button is pressed (with debouncing).
   * @return true if pressed.
   */
  bool pressed();
};