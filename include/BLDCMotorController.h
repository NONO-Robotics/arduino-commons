#pragma once
#include <BLDCMotor.h>
#include <WToSignedPWMConverter.h>

/**
 * @brief High-level controller for a single BLDC Motor.
 *
 * Provides translation from angular velocity (rad/s) to PWM signals, handling
 * deadzones and constraints.
 */
class BLDCMotorController {
private:
  WToSignedPWMConverter *wConverter;
  BLDCMotor *motor;
  float factor;

public:
  /**
   * @brief Construct a new BLDCMotor Controller.
   *
   * @param motor Pointer to the initialized BLDCMotor.
   * @param maxW Maximum angular velocity (rad/s) expected.
   * @param minPwm Minimum PWM value to start movement (deadzone compensation).
   * @param maxPwm Maximum PWM value allowed.
   */
  BLDCMotorController(BLDCMotor *motor, float maxW, int minPwm, int maxPwm, float factor = 1.0);

  /**
   * @brief Setup the controller.
   * @return Pointer to this controller instance.
   */
  BLDCMotorController *setup();

  /**
   * @brief Set speed in Radians per Second.
   * @param radsBySeg Angular velocity in rad/s.
   * @return Pointer to this controller instance.
   */
  BLDCMotorController *setRadsBySegSpeed(float radsBySeg);

  /**
   * @brief Direct PWM control (mainly for testing).
   * @param value Signed PWM value.
   * @return Pointer to this controller instance.
   */
  BLDCMotorController *setPwmSpeed(int value);

  /**
   * @brief Stop the motor.
   * @return Pointer to this controller instance.
   */
  BLDCMotorController *stop();
};