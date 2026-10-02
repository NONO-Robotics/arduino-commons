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
/**
 * @brief Converts AS5600 encoder count changes into filtered angular velocity.
 *
 * The estimator applies an EWMA filter and suppresses values within its configured dead zone.
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
   * @brief Creates an angular-velocity estimator with filtering and a dead zone.
   * @param alpha EWMA smoothing factor; defaults to DEFAULT_ALPHA.
   * @param deadZone Angular-velocity magnitude in rad/s treated as zero; defaults to DEFAULT_DEAD_ZONE.
   */
  EncoderAngularVelocityEstimator(double alpha = DEFAULT_ALPHA,
                                  float deadZone = DEFAULT_DEAD_ZONE)
      : filter(new Ewma(alpha)), deadZone(deadZone) {}

  /**
   * @brief Releases the estimator's EWMA filter.
   */
  ~EncoderAngularVelocityEstimator() { delete filter; }

  /**
   * @brief Converts an encoder count delta into signed angular velocity.
   *
   * @param valueDiff Signed change in 12-bit encoder counts.
   * @param deltaTimeMs Elapsed sampling time in milliseconds.
   * @param applyFilter True to apply the EWMA filter; defaults to true.
   * @return Signed angular velocity in rad/s, or zero for a non-positive interval or dead-zone value.
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
