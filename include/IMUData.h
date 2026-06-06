
#pragma once

/**
 * @brief Structure/Class holding IMU sensor data.
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
   * @brief Construct a new IMUData object.
   * Initializes all data fields to 0.0.
   */
  IMUData();

  /**
   * @brief Set the orientation quaternion.
   *
   * @param w W component.
   * @param x X component.
   * @param y Y component.
   * @param z Z component.
   */
  void setOrientation(double w, double x, double y, double z);

  /**
   * @brief Set the angular velocity.
   *
   * @param x Velocity around X-axis.
   * @param y Velocity around Y-axis.
   * @param z Velocity around Z-axis.
   */
  void setAngularVelocity(double x, double y, double z);

  /**
   * @brief Set the linear acceleration.
   *
   * @param x Acceleration along X-axis.
   * @param y Acceleration along Y-axis.
   * @param z Acceleration along Z-axis.
   */
  void setLinearAcceleration(double x, double y, double z);

  /**
   * @brief Get the X component of the orientation quaternion.
   * @return double Orientation X.
   */
  double getOrientationX();

  /**
   * @brief Get the Y component of the orientation quaternion.
   * @return double Orientation Y.
   */
  double getOrientationY();

  /**
   * @brief Get the Z component of the orientation quaternion.
   * @return double Orientation Z.
   */
  double getOrientationZ();

  /**
   * @brief Get the W component of the orientation quaternion.
   * @return double Orientation W.
   */
  double getOrientationW();

  /**
   * @brief Get the angular velocity around the X-axis.
   * @return double Angular velocity X (rad/s).
   */
  double getAngularVelocityX();

  /**
   * @brief Get the angular velocity around the Y-axis.
   * @return double Angular velocity Y (rad/s).
   */
  double getAngularVelocityY();

  /**
   * @brief Get the angular velocity around the Z-axis.
   * @return double Angular velocity Z (rad/s).
   */
  double getAngularVelocityZ();

  /**
   * @brief Get the linear acceleration along the X-axis.
   * @return double Linear acceleration X (m/s^2).
   */
  double getLinearAccelerationX();

  /**
   * @brief Get the linear acceleration along the Y-axis.
   * @return double Linear acceleration Y (m/s^2).
   */
  double getLinearAccelerationY();

  /**
   * @brief Get the linear acceleration along the Z-axis.
   * @return double Linear acceleration Z (m/s^2).
   */
  double getLinearAccelerationZ();
};