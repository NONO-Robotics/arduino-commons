#pragma once

#include <Arduino.h>

/**
 * @brief Controls a GPIO configured as a digital output.
 */
class DigitalOutput {
 public:
  /**
   * @brief Configures the GPIO as an output and initializes it to LOW.
   * @param pin GPIO number to control.
   */
  explicit DigitalOutput(uint8_t pin) : pin(pin) {
    pinMode(pin, OUTPUT);
    low();
  }

  /**
   * @brief Sets the GPIO to HIGH.
   */
  void high() { digitalWrite(pin, HIGH); }

  /**
   * @brief Sets the GPIO to LOW.
   */
  void low() { digitalWrite(pin, LOW); }

 private:
  uint8_t pin;
};
