#pragma once

#include "BLDCMotor.h"

/**
 * @brief Builder class for constructing BLDCMotor objects with fluent
 * interface.
 */
class BLDCMotorBuilder {
public:
  /**
   * @brief Constructor for BLDCMotorBuilder.
   * @param pwmPin Pin for PWM.
   * @param dirPin Pin for direction.
   * @param brakePin Pin for brake.
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
   * @brief Set the PWM channel.
   * @param ch Channel number.
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
   * @brief Invert the motor direction.
   * @return Pointer to this builder.
   */
  BLDCMotorBuilder *invertDirection() {
    this->_invertDirection = true;
    return this;
  }

  /**
   * @brief Build the BLDCMotor object.
   * @return Pointer to the new BLDCMotor instance.
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