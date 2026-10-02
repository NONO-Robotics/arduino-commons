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
 * @brief Configures a BLDC motor driver without initializing its hardware.
 * @param pwmPin GPIO pin carrying the PWM speed signal.
 * @param dirPin GPIO pin selecting motor direction.
 * @param brakePin GPIO pin controlling the motor brake.
 * @param resolutionInBits PWM resolution in bits; defaults to DEFAULT_RESOLUTION_IN_BITS.
 * @param channel LEDC channel used to generate PWM; defaults to DEFAULT_CHANNEL.
 * @param frequency PWM carrier frequency in Hz; defaults to DEFAULT_FREQUENCY.
 * @param invertDirection True to reverse the configured direction logic.
   */
  BLDCMotor(int pwmPin, int dirPin, int brakePin,
            int resolutionInBits = DEFAULT_RESOLUTION_IN_BITS,
            int channel = DEFAULT_CHANNEL, float frequency = DEFAULT_FREQUENCY,
            bool invertDirection = false);

  /**
 * @brief Initializes GPIO pins and configures the selected LEDC PWM channel.
 * @return Pointer to this initialized BLDCMotor instance.
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
 * @brief Returns the configured PWM resolution.
 * @return PWM resolution in bits.
   */
  int getResolutionInBits();

  /**
 * @brief Activates the motor driver's physical brake output.
 * @return Pointer to this BLDCMotor instance with braking enabled.
   */
  BLDCMotor *brake();

  /**
 * @brief Releases the motor driver's physical brake output.
 * @return Pointer to this BLDCMotor instance with braking disabled.
   */
  BLDCMotor *releaseBrake();

  /**
 * @brief Commands zero speed and activates the physical brake.
 * @return Pointer to this stopped BLDCMotor instance.
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
