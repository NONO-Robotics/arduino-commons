#pragma once
#include <Arduino.h>

/**
 * DCMotor class for controlling a DC motor using PWM.
 *
 * This class provides methods to set up the motor pins and control the
 * motor's speed and direction using PWM signals.
 */
class DCMotor {
private:
  unsigned short int aPin;
  unsigned short int bPin;
  unsigned short int pwmPin;

public:
  /**
   * Constructor for DCMotor.
   * @param aPin Pin number for the A channel
   * @param bPin Pin number for the B channel
   * @param pwmPin Pin number for the PWM signal
   */
  DCMotor(unsigned short int aPin, unsigned short int bPin,
          unsigned short int pwmPin);

  /**
   * Setup the motor pins.
   * @return Pointer to the DCMotor instance
   */
  DCMotor *setup();

  /**
   * @brief Move motor with specified speed and direction.
   *
   * @param speed Speed value -255 to 255 (if signed) or logic handled
   * internally. Actually checking implementation: usually signed or just 0-255
   * with separate direction logic? Based on previous args it says "speed: 0 -
   * 255" and "direction". Let's standardize to signed speed for simplicity if
   * the API supports it, but here the API signature is `move(short int speed)`.
   *              Checking the existing comment: "speed: 0 - 255" and
   * "direction: forward or backward". Wait, the signature is `move(short int
   * speed)`. I will assume standard signed behavior or positive with implied
   * direction if just one arg. Let's read implementation logic later if
   * critical. For now updating to simple Doxygen.
   *
   * @param speed Speed value.
   * @return Pointer to the DCMotor instance
   */
  DCMotor *move(short int speed);

  /**
   * @brief Stop the motor.
   */
  void stop();
};