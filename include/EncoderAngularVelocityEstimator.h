#pragma once
#include <Arduino.h>
#include <Ewma.h>
#include <math.h>

const float DEFAULT_ALPHA = 0.8;
const float DEFAULT_DEAD_ZONE = 0.55;

/**
 * @brief Calculates angular velocity from encoder steps over time.
 * 
 * Usage Context: Essential for processing noisy raw data from AS5600 
 * magnetic encoders (e.g., in 4w-outdoor-robot-ros-w-publisher).
 * Implements an Exponentially Weighted Moving Average (EWMA) filter
 * to smooth out spikes and provide stable velocity estimates (rad/s)
 * for reliable odometry calculation.
 */
class EncoderAngularVelocityEstimator {

private:
  // Conversion factor from raw encoder units (12-bit, 4096 steps) to radians.
  // 2 * PI radians in a full rotation.
  static constexpr float RAW_TO_RADIANS = (2.0f * M_PI) / 4096.0f;

  Ewma *filter;
  float deadZone;

public:
  /**
   * @brief Constructor.
   * @param alpha Filter smoothing factor.
   * @param deadZone Deadzone for velocity.
   */
  EncoderAngularVelocityEstimator(double alpha = DEFAULT_ALPHA,
                                  float deadZone = DEFAULT_DEAD_ZONE)
      : filter(new Ewma(alpha)), deadZone(deadZone) {}

  ~EncoderAngularVelocityEstimator() { delete filter; }

  /**
   * @brief Calculate angular velocity (W) in rad/s.
   *
   * @param valueDiff Difference in encoder steps.
   * @param deltaTimeMs Time difference in milliseconds.
   * @param applyFilter Whether to apply EWMA filter.
   * @return Angular velocity in rad/s.
   */
  float getWInRadBySec(float valueDiff, float deltaTimeMs,
                       bool applyFilter = true) {
    if (deltaTimeMs <= 0)
      return 0.0f;

    // Convert raw unit difference to radians
    float deltaRadians = valueDiff * RAW_TO_RADIANS;

    // Convert delta time to seconds
    float deltaTimeSec = deltaTimeMs / 1000.0f;

    // Calculate angular velocity in radians per second
    float wInRadBySec = deltaRadians / deltaTimeSec;

    if (applyFilter) {
      wInRadBySec = filter->filter(wInRadBySec);
    }

    return fabs(wInRadBySec) <= deadZone ? 0.0f : wInRadBySec;
  }
};