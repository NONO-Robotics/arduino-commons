
#pragma once

/**
 * @brief Stores orientation, angular velocity, and linear acceleration samples.
 *
 * This class stores the orientation quaternion, angular velocity, and linear
 * acceleration data from an IMU sensor (e.g., BNO08x).
 */
class IMUData
{
private:
  double orientation_x; ///< X component of the orientation quaternion.
  double orientation_y; ///< Y component of the orientation quaternion.
  double orientation_z; ///< Z component of the orientation quaternion.
  double orientation_w; ///< W component of the orientation quaternion.

  double angular_velocity_x; ///< Angular velocity around the X-axis (rad/s).
  double angular_velocity_y; ///< Angular velocity around the Y-axis (rad/s).
  double angular_velocity_z; ///< Angular velocity around the Z-axis (rad/s).

  double linear_acceleration_x; ///< Linear acceleration along the X-axis (m/s^2).
  double linear_acceleration_y; ///< Linear acceleration along the Y-axis (m/s^2).
  double linear_acceleration_z; ///< Linear acceleration along the Z-axis (m/s^2).

public:
  /**
   * @brief Create an IMU sample with every field initialized to zero.
   */
  IMUData();

  /**
   * @brief Store an orientation quaternion from the latest sensor sample.
   *
   * @param w Scalar quaternion component (unitless).
   * @param x X quaternion component (unitless).
   * @param y Y quaternion component (unitless).
   * @param z Z quaternion component (unitless).
   */
  void setOrientation(double w, double x, double y, double z);

  /**
   * @brief Store angular velocity around each sensor axis.
   *
   * @param x Angular velocity around X-axis in rad/s.
   * @param y Angular velocity around Y-axis in rad/s.
   * @param z Angular velocity around Z-axis in rad/s.
   */
  void setAngularVelocity(double x, double y, double z);

  /**
   * @brief Store linear acceleration along each sensor axis.
   *
   * @param x Linear acceleration along X-axis in m/s^2.
   * @param y Linear acceleration along Y-axis in m/s^2.
   * @param z Linear acceleration along Z-axis in m/s^2.
   */
  void setLinearAcceleration(double x, double y, double z);

  /**
   * @brief Return the X component of the stored orientation quaternion.
   * @return Unitless X quaternion component.
   */
  double getOrientationX();

  /**
   * @brief Return the Y component of the stored orientation quaternion.
   * @return Unitless Y quaternion component.
   */
  double getOrientationY();

  /**
   * @brief Return the Z component of the stored orientation quaternion.
   * @return Unitless Z quaternion component.
   */
  double getOrientationZ();

  /**
   * @brief Return the scalar component of the stored orientation quaternion.
   * @return Unitless W quaternion component.
   */
  double getOrientationW();

  /**
   * @brief Return angular velocity around the X-axis.
   * @return Angular velocity in rad/s.
   */
  double getAngularVelocityX();

  /**
   * @brief Return angular velocity around the Y-axis.
   * @return Angular velocity in rad/s.
   */
  double getAngularVelocityY();

  /**
   * @brief Return angular velocity around the Z-axis.
   * @return Angular velocity in rad/s.
   */
  double getAngularVelocityZ();

  /**
   * @brief Return linear acceleration along the X-axis.
   * @return Linear acceleration in m/s^2.
   */
  double getLinearAccelerationX();

  /**
   * @brief Return linear acceleration along the Y-axis.
   * @return Linear acceleration in m/s^2.
   */
  double getLinearAccelerationY();

  /**
   * @brief Return linear acceleration along the Z-axis.
   * @return Linear acceleration in m/s^2.
   */
  double getLinearAccelerationZ();
};
