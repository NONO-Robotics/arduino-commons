#pragma once
#include <Arduino.h>

/**
 * @brief Reads an active-low GPIO button with a fixed 50 ms debounce interval.
 */
class Button {
private:
  uint8_t pin;
  bool previousState;
  unsigned long lastDebounceTime = 0;
  const unsigned long debounceTime = 50; ///< Debounce time in milliseconds

public:
  /**
   * @brief Configures a button reader for one GPIO pin.
   * @param p GPIO pin connected to the button.
   */
  Button(uint8_t p);

  /**
   * @brief Configures the button pin as INPUT_PULLUP.
   */
  void init();

  /**
   * @brief Reports a debounced button press and updates the saved pin state.
   * @return True when the active-low button is pressed after debouncing.
   */
  bool pressed();
};
