#pragma once
#include "BLDCMotorBuilder.h"
#include "BLDCMotorController.h"
#include "FourWheelAngularSpeed.h"

/**
 * @brief Controller for a 4-wheel robot using BLDC motors.
 *
 * This class coordinates four BLDC motors (Front-Left, Front-Right, Back-Left,
 * Back-Right) to achieve desired angular velocities for the robot base.
 * 
 * Usage Context: Central controller in outdoor/4x4 independent drive robots
 * (e.g. 4w-outdoor-robot-ros-movement). It receives target wheel speeds
 * (calculated from Twist messages via FWAngularSpeedWriter) and applies them
 * simultaneously to all four wheels.
 */
class FourWheelBLDCController {
private:
  BLDCMotorController *motorFrontRightController;
  BLDCMotorController *motorFrontLeftController;
  BLDCMotorController *motorBackLeftController;
  BLDCMotorController *motorBackRightController;

 public:
  /**
   * @brief Constructs the four-wheel actuator from one complete configuration.
   *
   * @param frontRight Settings for logical front-right motor.
   * @param frontLeft Settings for logical front-left motor.
   * @param backRight Settings for logical back-right motor.
   * @param backLeft Settings for logical back-left motor.
   */
  FourWheelBLDCController(const BLDCMotorSettings &frontRight,
                          const BLDCMotorSettings &frontLeft,
                          const BLDCMotorSettings &backRight,
                          const BLDCMotorSettings &backLeft);

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
