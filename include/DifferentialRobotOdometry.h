#pragma once

#include "FourWheelsRobotW.h"
#include "TwoWheelsRobotW.h"

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
   * @brief Update from a two-wheel robot state.
   * @param robotW State of the two-wheel robot.
   */
  void updateFrom(TwoWheelsRobotW robotW) {
    updateFrom({robotW.left, robotW.right, robotW.left, robotW.right});
  }

  /**
   * @brief Update from a four-wheel robot state (averaging sides).
   * @param robotW State of the four-wheel robot.
   */
  void updateFrom(FourWheelsRobotW robotW) {
    leftW = (robotW.fl + robotW.bl) / 2.0f;
    rightW = (robotW.fr + robotW.br) / 2.0f;
  }
};
