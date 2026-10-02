#pragma once
#include <BLDCMotor.h>
#include <WToSignedPWMConverter.h>

/** @brief Physical and conversion settings for one BLDC motor controller. */
struct BLDCMotorSettings {
  /** @brief GPIO supplying PWM duty to this motor. */
  int pwmPin;
  /** @brief GPIO selecting this motor direction. */
  int directionPin;
  /** @brief GPIO activating this motor brake. */
  int brakePin;
  /** @brief LEDC channel assigned to this motor PWM output. */
  int pwmChannel;
  /** @brief Reverses electrical direction to match logical wheel direction. */
  bool invertDirection;
  /** @brief Largest permitted logical wheel speed magnitude. */
  float maxW;
  /** @brief PWM duty used for the smallest nonzero motor command. */
  int minPwm;
  /** @brief PWM duty used for the largest motor command. */
  int maxPwm;
  /** @brief Output multiplier applied to this motor. */
  float factor;
};

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
   * @brief Creates a controller that maps angular-speed targets to motor PWM.
   *
   * @param motor Pointer to the initialized BLDCMotor to command.
   * @param maxW Maximum commanded angular velocity in rad/s.
   * @param minPwm Minimum PWM magnitude that overcomes the motor dead zone.
   * @param maxPwm Maximum permitted PWM magnitude.
   * @param factor Scale applied to the angular-speed command; defaults to 1.0.
   */
  BLDCMotorController(BLDCMotor *motor, float maxW, int minPwm, int maxPwm, float factor = 1.0);

  /**
   * @brief Initializes the controlled motor and its speed-to-PWM converter.
   * @return Pointer to this initialized controller instance.
   */
  BLDCMotorController *setup();

  /**
   * @brief Converts an angular-speed target to PWM and commands the motor.
   * @param radsBySeg Requested signed angular velocity in rad/s.
   * @return Pointer to this controller instance.
   */
  BLDCMotorController *setRadsBySegSpeed(float radsBySeg);

  /**
   * @brief Sends a signed PWM command directly to the motor.
   * @param value Signed PWM duty-cycle value.
   * @return Pointer to this controller instance.
   */
  BLDCMotorController *setPwmSpeed(int value);

  /**
   * @brief Stops the controlled motor.
   * @return Pointer to this controller after stopping its motor.
   */
  BLDCMotorController *stop();
};
