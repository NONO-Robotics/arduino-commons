#pragma once
#include <Arduino.h>

/**
 * @brief Converts robot physical limits and PWM resolution into control values.
 * 
 * Usage Context: Bridges the gap between kinematic mathematics (rad/s) and
 * the physical motors' PWM. Handles constraints such as motor deadzones 
 * (minimum PWM required to move) and maximum limits to protect hardware.
 */
class WToSignedPWMConverter {
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
  int convert(float w);
};
