#pragma once

/**
 * @brief Immutable snapshot of four wheel angular speeds.
 *
 * Preserves received side averages and individual wheel angular speeds.
 */
class WheelSpeeds {
 private:
  float averageLeftWInRad_;
  float averageRightWInRad_;
  float flWInRad_;
  float frWInRad_;
  float blWInRad_;
  float brWInRad_;

 public:
  /**
   * @brief Creates wheel speeds from side averages and individual wheel speeds.
   *
   * @param averageLeftWInRad Left-side average angular speed, in rad/s.
   * @param averageRightWInRad Right-side average angular speed, in rad/s.
   * @param flWInRad Front-left angular speed, in rad/s.
   * @param frWInRad Front-right angular speed, in rad/s.
   * @param blWInRad Back-left angular speed, in rad/s.
   * @param brWInRad Back-right angular speed, in rad/s.
   */
  constexpr WheelSpeeds(float averageLeftWInRad, float averageRightWInRad,
                        float flWInRad, float frWInRad, float blWInRad,
                        float brWInRad)
      : averageLeftWInRad_(averageLeftWInRad),
        averageRightWInRad_(averageRightWInRad), flWInRad_(flWInRad),
        frWInRad_(frWInRad), blWInRad_(blWInRad), brWInRad_(brWInRad) {}

  /** @returns Left-side average angular speed, in rad/s. */
  constexpr float getAverageLeftWInRad() const { return averageLeftWInRad_; }
  /** @returns Right-side average angular speed, in rad/s. */
  constexpr float getAverageRightWInRad() const { return averageRightWInRad_; }
  /** @returns Front-left angular speed, in rad/s. */
  constexpr float getFlWInRad() const { return flWInRad_; }
  /** @returns Front-right angular speed, in rad/s. */
  constexpr float getFrWInRad() const { return frWInRad_; }
  /** @returns Back-left angular speed, in rad/s. */
  constexpr float getBlWInRad() const { return blWInRad_; }
  /** @returns Back-right angular speed, in rad/s. */
  constexpr float getBrWInRad() const { return brWInRad_; }
};
