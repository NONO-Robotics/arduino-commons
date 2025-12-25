#pragma once
#include "NumberUtils.h"
#include <Arduino.h>

/**
 * @brief Converts robot physical limits and PWM resolution into control values.
 */
class WToSignedPWMConverter {
private:
  float maxW;
  int minPwm;
  int maxPwmLimit;
  int maxPwm;

public:
  /**
   * @brief Constructor.
   * @param pwmResolutionInBits PWM resolution (Recommended: 12 bits)
   * @param minPwm Minimum deadzone for motor to start turning
   */
  WToSignedPWMConverter(float maxW, int pwmResolutionInBits, int minPwm,
                        int maxPwm = 0);

  /**
   * @brief Convert angular velocity to signed PWM.
   * @param w Angular velocity.
   * @return Signed PWM value.
   */
  int convert(float w);
};