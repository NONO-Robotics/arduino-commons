#pragma once
#include <Arduino.h>

/** 
 * DCMotor class for controlling a DC motor using PWM.
 *
 * This class provides methods to set up the motor pins and control the
 * motor's speed and direction using PWM signals.
 */
class DCMotor
{
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
    DCMotor(
        unsigned short int aPin,
        unsigned short int bPin,
        unsigned short int pwmPin);

    /**
     * Setup the motor pins.
     * @return Pointer to the DCMotor instance
     */
    DCMotor *setup();

    /**
      Move motor.

      Args:
        - speed: 0 - 255, adjust as desired.
        - direction: forward or backward.
    */
    DCMotor *move(short int speed);

    /**
     * Stop the motor.
     */
    void stop();
};