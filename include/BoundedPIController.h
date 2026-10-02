#pragma once

/**
 * @brief Proportional-integral controller with bounded output and integral.
 *
 * The controller is independent from Arduino APIs so its signed-error behavior
 * can be tested on a host machine.
 */
class BoundedPIController {
private:
  float proportionalGain;
  float integralGain;
  float maxCorrection;
  float maxIntegral;
  float integral;

  float clamp(float value, float minimum, float maximum) const;

public:
  /**
   * @brief Creates a PI controller with bounded output and accumulated error.
   *
   * @param proportionalGain Gain applied to the current signed error.
   * @param integralGain Gain applied to the accumulated signed error.
   * @param maxCorrection Maximum absolute returned correction.
   * @param maxIntegral Maximum absolute accumulated error.
   */
  BoundedPIController(float proportionalGain, float integralGain,
                      float maxCorrection, float maxIntegral);

  /**
   * @brief Computes a bounded correction from signed target and measurement.
   *
   * @param target Requested value.
   * @param measured Measured value using the same sign convention as target.
   * @param deltaSeconds Elapsed time since the previous update in seconds.
   * @return Bounded signed correction.
   */
  float update(float target, float measured, float deltaSeconds);

  /**
   * @brief Computes a bounded correction with additional output limits.
   *
   * Limits are applied together with the configured maximum correction. They
   * let a caller prevent a correction from crossing a target's safety bound.
   *
   * @param target Requested value.
   * @param measured Measured value using the same sign convention as target.
   * @param deltaSeconds Elapsed time since the previous update in seconds.
   * @param minimumCorrection Minimum permitted correction.
   * @param maximumCorrection Maximum permitted correction.
   * @return Bounded signed correction.
   */
  float update(float target, float measured, float deltaSeconds,
               float minimumCorrection, float maximumCorrection);

  /**
   * @brief Clears the accumulated error without changing configured limits or gains.
   */
  void reset();
};
