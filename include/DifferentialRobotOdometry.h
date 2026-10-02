#pragma once

#include "FourWheelAngularSpeed.h"

/**
 * @brief Class for tracking differential robot odometry in terms of wheel
 * angular velocities.
 */
/**
 * @brief Stores left and right angular velocities derived from four-wheel state.
 */
class DifferentialRobotOdometry {
private:
  float leftW, rightW;

public:
  /**
   * @brief Creates an odometry state with both side velocities set to zero.
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
   * @brief Averages front and rear wheel speeds into left and right velocities.
   * @param robotW Four-wheel angular-speed state in rad/s.
   */
  void updateFrom(FourWheelAngularSpeed robotW) {
    leftW = (robotW.getFlWInRad() + robotW.getBlWInRad()) / 2.0f;
    rightW = (robotW.getFrWInRad() + robotW.getBrWInRad()) / 2.0f;
  }
};
