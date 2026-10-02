#pragma once
#include <Arduino.h>

/**
 * @brief Controls a bidirectional DC motor through two direction pins and PWM.
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
   * @brief Configures the GPIO pins used by a DC motor driver.
   * @param aPin GPIO pin for direction channel A.
   * @param bPin GPIO pin for direction channel B.
   * @param pwmPin GPIO pin carrying the PWM signal.
   */
  DCMotor(unsigned short int aPin, unsigned short int bPin,
          unsigned short int pwmPin);

  /**
   * @brief Configures the motor GPIO pins for output.
   * @return Pointer to this initialized DCMotor instance.
   */
  DCMotor *setup();

  /**
   * @brief Sets motor direction from the sign and PWM magnitude from the speed.
   * @param speed Signed PWM command; non-negative values select channel A and
   * negative values select channel B.
   * @return Pointer to this DCMotor instance after applying the command.
   */
  DCMotor *move(short int speed);

  /**
   * @brief Stops PWM output to the motor.
   */
  void stop();
};
