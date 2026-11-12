#pragma once
#include <Arduino.h>

/** @brief Converts a signed wheel angular-speed target to signed PWM. */
class SpeedToPwmConverter {
 public:
  virtual ~SpeedToPwmConverter() = default;

  /**
   * @brief Converts an angular-speed target to a bounded signed PWM command.
   * @param targetW Requested signed angular velocity in rad/s.
   * @return Signed PWM duty-cycle value; zero stops the motor.
   */
  virtual int convert(float targetW) const = 0;
};

/**
 * @brief Converts robot physical limits and PWM resolution into control values.
 * 
 * Usage Context: Bridges the gap between kinematic mathematics (rad/s) and
 * the physical motors' PWM. Handles constraints such as motor deadzones 
 * (minimum PWM required to move) and maximum limits to protect hardware.
 */
class WToSignedPWMConverter : public SpeedToPwmConverter {
private:
  float maxW;
  int minPwm;
  int maxPwmLimit;
  int maxPwm;

public:
  /**
   * @brief Creates a converter from signed angular velocity to signed PWM.
   * @param maxW Maximum angular-velocity magnitude in rad/s.
   * @param pwmResolutionInBits PWM resolution in bits.
   * @param minPwm Minimum PWM magnitude that overcomes the motor dead zone.
   * @param maxPwm Maximum PWM magnitude; zero uses the PWM-resolution limit.
   */
  WToSignedPWMConverter(float maxW, int pwmResolutionInBits, int minPwm,
                        int maxPwm = 0);

  /**
   * @brief Map an angular-velocity request to a bounded signed PWM duty cycle.
   * @param w Requested angular velocity in rad/s.
   * @return Signed PWM duty-cycle value; sign indicates requested direction.
   */
  int convert(float w) const override;
};

/** @brief Interpolates a measured wheel-speed to PWM calibration table. */
class TableSpeedToPwmConverter final : public SpeedToPwmConverter {
 public:
  /** @brief One nonnegative measured wheel-speed and PWM pair. */
  struct Point {
    int pwm;
    float w;
  };

  /**
   * @brief Creates a converter over ascending wheel-speed calibration points.
   * @param points Measured nonnegative speed-to-PWM points owned by firmware.
   * @param pointCount Number of calibration points.
   */
  TableSpeedToPwmConverter(const Point* points, unsigned int pointCount);

  int convert(float targetW) const override;

 private:
  const Point* points_;
  unsigned int pointCount_;
};
