#pragma once
#include <Arduino.h>

const int DEFAULT_CHANNEL = 0;
const float DEFAULT_FREQUENCY = 20000; // Raised to 20kHz to silence the motor
const int DEFAULT_RESOLUTION_IN_BITS = 11;

/**
 * @brief Class for controlling a BLDC motor using PWM and direction pins.
 */
class BLDCMotor {
public:
  /**
   * @brief Constructor for BLDCMotor.
   * @param pwmPin Pin for PWM speed control.
   * @param dirPin Pin for direction control.
   * @param brakePin Pin for brake control.
   * @param resolutionInBits PWM resolution in bits (default 11).
   * @param channel LEDC channel for PWM generation (default 0).
   * @param frequency PWM frequency in Hz (default 20000).
   * @param invertDirection If true, inverts the motor direction logic.
   */
  BLDCMotor(int pwmPin, int dirPin, int brakePin,
            int resolutionInBits = DEFAULT_RESOLUTION_IN_BITS,
            int channel = DEFAULT_CHANNEL, float frequency = DEFAULT_FREQUENCY,
            bool invertDirection = false);

  /**
   * @brief Initialize the motor pins and PWM channel.
   * @return Pointer to this BLDCMotor instance.
   */
  BLDCMotor *setup();

  /**
   * @brief Set the motor speed via PWM.
   * @param pwm PWM duty cycle value (signed). Positive for forward, negative
   * for reverse.
   * @return Pointer to this BLDCMotor instance.
   */
  BLDCMotor *setPwmSpeed(int pwm);

  /**
   * @brief Get the configured PWM resolution.
   * @return Resolution in bits.
   */
  int getResolutionInBits();

  /**
   * @brief Activate the physical brake.
   * @return Pointer to this BLDCMotor instance.
   */
  BLDCMotor *brake();

  /**
   * @brief Release the physical brake.
   * @return Pointer to this BLDCMotor instance.
   */
  BLDCMotor *releaseBrake();

  /**
   * @brief Stop the motor immediately (activates brake).
   * @return Pointer to this BLDCMotor instance.
   */
  BLDCMotor *stop(); // Removed float pauseInMs

private:
  int pwmPin;
  int dirPin;
  int brakePin;
  int currentSpeed;
  int channel;
  float frequency;
  int resolutionInBits;
  int pwmMax;
  bool invertDirection;

  const int DIR_FORWARD = LOW;
  const int DIR_REVERSE = HIGH;
};