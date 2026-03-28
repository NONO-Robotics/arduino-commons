#pragma once
#include "BLDCMotorBuilder.h"
#include "BLDCMotorController.h"
#include "FourWheelAngularSpeed.h"

/**
 * @brief Controller for a 4-wheel robot using BLDC motors.
 *
 * This class coordinates four BLDC motors (Front-Left, Front-Right, Back-Left,
 * Back-Right) to achieve desired angular velocities for the robot base.
 */
class FourWheelBLDCController {
private:
  BLDCMotorController *motorFrontRightController;
  BLDCMotorController *motorFrontLeftController;
  BLDCMotorController *motorBackLeftController;
  BLDCMotorController *motorBackRightController;

public:
  /**
   * @brief Construct a new Four Wheel BLDC Controller object.
   *
   * @param maxW Maximum angular velocity for the wheels (rad/s).
   * @param minPwm Minimum PWM signal value.
   * @param maxPwm Maximum PWM signal value.
   * @param pinPwmFrontRight PWM pin for Front-Right motor.
   * @param pinDirFrontRight Direction pin for Front-Right motor.
   * @param pinBrakeFrontRight Brake pin for Front-Right motor.
   * @param pinPwmFrontLeft PWM pin for Front-Left motor.
   * @param pinDirFrontLeft Direction pin for Front-Left motor.
   * @param pinBrakeFrontLeft Brake pin for Front-Left motor.
   * @param pinPwmBackRight PWM pin for Back-Right motor.
   * @param pinDirBackRight Direction pin for Back-Right motor.
   * @param pinBrakeBackRight Brake pin for Back-Right motor.
   * @param pinPwmBackLeft PWM pin for Back-Left motor.
   * @param pinDirBackLeft Direction pin for Back-Left motor.
   * @param pinBrakeBackLeft Brake pin for Back-Left motor.
   */
  FourWheelBLDCController(float maxW, int minPwm, int maxPwm,
                          int pinPwmFrontRight, int pinDirFrontRight,
                          int pinBrakeFrontRight, int pinPwmFrontLeft,
                          int pinDirFrontLeft, int pinBrakeFrontLeft,
                          int pinPwmBackRight, int pinDirBackRight,
                          int pinBrakeBackRight, int pinPwmBackLeft,
                          int pinDirBackLeft, int pinBrakeBackLeft,
                          float frontFactor = 1.0, float backFactor = 1.0);

  /**
   * @brief Stops all four motors immediately.
   */
  void stop();

  /**
   * @brief Applies the target angular speeds to each wheel.
   *
   * @param fwAngularSpeed Object containing the target speed for each wheel in
   * rad/s.
   */
  void applySpeed(const FourWheelAngularSpeed &fwAngularSpeed);
};