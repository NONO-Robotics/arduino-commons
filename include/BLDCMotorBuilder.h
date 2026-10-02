#pragma once

#include "BLDCMotor.h"

/**
 * @brief Builds configured BLDCMotor instances through a fluent interface.
 */
class BLDCMotorBuilder {
public:
  /**
   * @brief Starts a motor configuration with its required GPIO pins.
   * @param pwmPin GPIO pin carrying the PWM speed signal.
   * @param dirPin GPIO pin selecting motor direction.
   * @param brakePin GPIO pin controlling the motor brake.
   */
  BLDCMotorBuilder(int pwmPin, int dirPin, int brakePin)
      : pwmPin(pwmPin), dirPin(dirPin), brakePin(brakePin),
        resolutionInBits(DEFAULT_RESOLUTION_IN_BITS), channel(DEFAULT_CHANNEL),
        frequency(DEFAULT_FREQUENCY), _invertDirection(false) {}

  /**
   * @brief Set the PWM resolution in bits.
   * @param bits Resolution bits (1-16).
   * @return Pointer to this builder.
   */
  BLDCMotorBuilder *setResolutionInBits(int bits) {
    if (bits <= 0 || bits > 16) { // Validation example
      // You could throw an exception or log an error
    }
    this->resolutionInBits = bits;
    return this;
  }

  /**
   * @brief Selects the LEDC channel used by the built motor.
   * @param ch LEDC channel number.
   * @return Pointer to this builder.
   */
  BLDCMotorBuilder *setChannel(int ch) {
    this->channel = ch;
    return this;
  }

  /**
   * @brief Set the PWM frequency.
   * @param freq Frequency in Hz.
   * @return Pointer to this builder.
   */
  BLDCMotorBuilder *setFrequency(float freq) {
    this->frequency = freq;
    return this;
  }

  /**
   * @brief Reverses the direction logic of the built motor.
   * @return Pointer to this builder with direction inversion enabled.
   */
  BLDCMotorBuilder *invertDirection() {
    this->_invertDirection = true;
    return this;
  }

  /**
   * @brief Allocates a BLDCMotor using the accumulated configuration.
   * @return Pointer to the newly allocated BLDCMotor instance.
   * @throws std::runtime_error if pin configuration is invalid.
   */
  BLDCMotor *build() {
    if (pwmPin < 0 || dirPin < 0 || brakePin < 0) {
      throw std::runtime_error("Pins must be non-negative.");
    }

    return new BLDCMotor(pwmPin, dirPin, brakePin, resolutionInBits, channel,
                         frequency, _invertDirection);
  }

private:
  int pwmPin;
  int dirPin;
  int brakePin;

  int resolutionInBits;
  int channel;
  float frequency;
  bool _invertDirection;
};
