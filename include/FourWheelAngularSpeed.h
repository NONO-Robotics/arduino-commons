#pragma once

/**
 * @brief Class for storing and updating the individual angular speeds of a 4-wheel robot.
 *
 * This class encapsulates the angular velocities (in radians per second) for all four
 * wheels (Front Left, Front Right, Back Left, and Back Right) of a mobile robot.
 */
class FourWheelAngularSpeed {
private:
  float fl; ///< Front Left wheel speed in rad/s.
  float fr; ///< Front Right wheel speed in rad/s.
  float bl; ///< Back Left wheel speed in rad/s.
  float br; ///< Back Right wheel speed in rad/s.

public:
  /**
   * @brief Construct a new FourWheelAngularSpeed object.
   * Initializes all wheel speeds to 0.0.
   */
  FourWheelAngularSpeed();

  /**
   * @brief Set wheel speeds in rad/s.
   * @param fl Front left wheel speed in rad/s.
   * @param fr Front right wheel speed in rad/s.
   * @param bl Rear left wheel speed in rad/s.
   * @param br Rear right wheel speed in rad/s.
   */
  void updateFrom(float fl, float fr, float bl, float br);

  /**
   * @brief Check whether every wheel speed magnitude is below a threshold.
   * @param threshold Maximum exclusive wheel speed magnitude in rad/s.
   * @return True when all four wheel speed magnitudes are strictly below the threshold.
   */
  bool lessThan(float threshold) const;

  /**
   * @brief Get average angular speed of left-side wheels in rad/s.
   * @return Signed arithmetic average of front-left and back-left wheel speeds in rad/s.
   */
  float getAverageLeftWInRad() const;

  /**
   * @brief Get average angular speed of right-side wheels in rad/s.
   * @return Signed arithmetic average of front-right and back-right wheel speeds in rad/s.
   */
  float getAverageRightWInRad() const;

  /**
   * @brief Get front left wheel speed in rad/s.
   * @return Front left wheel speed in rad/s.
   */
  float getFlWInRad() const;

  /**
   * @brief Get front right wheel speed in rad/s.
   * @return Front right wheel speed in rad/s.
   */
  float getFrWInRad() const;

  /**
   * @brief Get back left wheel speed in rad/s.
   * @return Back left wheel speed in rad/s.
   */
  float getBlWInRad() const;

  /**
   * @brief Get back right wheel speed in rad/s.
   * @return Back right wheel speed in rad/s.
   */
  float getBrWInRad() const;

  /**
   * @brief Set front left wheel speed.
   * @param w Speed in rad/s.
   * @return The updated speed in rad/s.
   */
  float setFlWInRad(float w);

  /**
   * @brief Set front right wheel speed.
   * @param w Speed in rad/s.
   * @return The updated speed in rad/s.
   */
  float setFrWInRad(float w);

  /**
   * @brief Set back left wheel speed.
   * @param w Speed in rad/s.
   * @return The updated speed in rad/s.
   */
  float setBlWInRad(float w);

  /**
   * @brief Set back right wheel speed.
   * @param w Speed in rad/s.
   * @return The updated speed in rad/s.
   */
  float setBrWInRad(float w);
};
