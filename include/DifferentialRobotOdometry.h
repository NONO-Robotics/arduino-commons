#pragma once

#include "FourWheelAngularSpeed.h"

/**
 * @brief Class for tracking differential robot odometry in terms of wheel
 * angular velocities.
 */
class DifferentialRobotOdometry {
private:
  float leftW, rightW;

public:
  /**
   * @brief Constructor for DifferentialRobotOdometry.
   * Initializes angular velocities to zero.
   */
  DifferentialRobotOdometry() {
    leftW = 0.0;
    rightW = 0.0;
  }

  /**
   * @brief Get left wheel angular velocity.
   * @return Angular velocity in rad/s.
   */
  float getLeftWInRad() const { return leftW; }

  /**
   * @brief Get right wheel angular velocity.
   * @return Angular velocity in rad/s.
   */
  float getRightWInRad() const { return rightW; }

  /**
   * @brief Update from a four-wheel robot state (averaging sides).
   * @param robotW State of the four-wheel robot.
   */
  void updateFrom(FourWheelAngularSpeed robotW) {
    leftW = (robotW.getFlWInRad() + robotW.getBlWInRad()) / 2.0f;
    rightW = (robotW.getFrWInRad() + robotW.getBrWInRad()) / 2.0f;
  }
};
