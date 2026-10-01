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
   * @brief Constructs a bounded PI controller.
   *
   * @param proportionalGain Proportional gain.
   * @param integralGain Integral gain.
   * @param maxCorrection Maximum absolute output correction.
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
   * @brief Clears accumulated error.
   */
  void reset();
};
