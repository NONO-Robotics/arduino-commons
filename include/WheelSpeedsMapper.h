#pragma once

#include <cstddef>

#include <DifferentialRobotOdometry.h>
#include <FourWheelAngularSpeed.h>

/**
 * @brief Maps odometry and individual wheel speeds to diagnostic order.
 */
class WheelSpeedsMapper {
public:
  static constexpr size_t Length = 6;

  /**
   * @brief Fills a preallocated diagnostic buffer.
   *
   * @param wheelSpeeds Buffer populated as left, right, FL, FR, BL, BR.
   * @param robotOdometry Differential odometry containing left and right speeds.
   * @param angularSpeed Individual wheel angular speeds.
   */
  static void toArray(float (&wheelSpeeds)[Length],
                      const DifferentialRobotOdometry &robotOdometry,
                      const FourWheelAngularSpeed &angularSpeed) {
    wheelSpeeds[0] = robotOdometry.getLeftWInRad();
    wheelSpeeds[1] = robotOdometry.getRightWInRad();
    wheelSpeeds[2] = angularSpeed.getFlWInRad();
    wheelSpeeds[3] = angularSpeed.getFrWInRad();
    wheelSpeeds[4] = angularSpeed.getBlWInRad();
    wheelSpeeds[5] = angularSpeed.getBrWInRad();
  }
};
