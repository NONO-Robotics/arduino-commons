#pragma once

#include <BoundedPIController.h>

/** @brief Per-wheel feedback calibration and enablement. */
struct WheelFeedbackSettings {
  /** @brief Enables PI correction for this calibrated wheel. */
  const bool enabled;
  /** @brief Converts physical encoder speed to command-speed units. */
  const float speedScale;
  /** @brief Immediate correction gain for current wheel-speed error. */
  const float proportionalGain;
  /** @brief Accumulated correction gain for persistent wheel-speed error. */
  const float integralGain;
  /** @brief Largest permitted absolute PI correction. */
  const float maxCorrection;
  /** @brief Largest permitted absolute accumulated PI error. */
  const float maxIntegral;
};

/**
 * @brief Regulates one wheel target with optional bounded PI feedback.
 *
 * Disabled channels pass targets through unchanged. Enabled channels use the
 * latest normalized wheel measurement and retain a minimum nonzero target so
 * PI correction cannot request PWM below the physical drive threshold.
 */
class WheelSpeedFeedbackController {
 private:
  BoundedPIController controller_;
  float measuredSpeed_;
  bool hasFeedback_;
  bool enabled_;
  unsigned long lastFeedbackMs_;
  unsigned long lastControlMs_;
  bool hasUpdateTime_;
  bool needsFreshCommand_;
  bool needsFreshWheelSpeeds_;

 public:
  /**
   * @brief Creates one wheel feedback channel.
   *
   * @param settings Enablement and bounded PI calibration for this wheel.
   */
  explicit WheelSpeedFeedbackController(const WheelFeedbackSettings &settings);

  /**
   * @brief Records a motion command received after a feedback fault.
   *
   * Each wheel requires this event and valid wheel speeds after a fault before
   * it may use feedback for a nonzero target.
   */
  void receiveFreshCommand();

  /**
   * @brief Stores timestamped, normalized wheel speeds for this wheel.
   *
   * @param measuredSpeed Wheel speed in same units and sign convention as target.
   * @param timestampMs Time at which valid wheel speeds were received.
   */
  void updateFeedback(float measuredSpeed, unsigned long timestampMs);

  /**
   * @brief Latches a feedback fault and clears PI state.
   *
   * Recovery requires a later fresh command and valid wheel speeds.
   */
  void fault();

  /**
   * @brief Checks whether an active target may safely use this feedback channel.
   *
   * Disabled channels and zero targets are safe. A stale recovered measurement
   * becomes a new fault, so recovery again requires both fresh inputs.
   *
   * @param target Requested signed wheel speed.
   * @param nowMs Current monotonic timestamp.
   * @param feedbackTimeoutMs Maximum valid diagnostic age.
   * @return True when target can safely be controlled.
   */
  bool canSafelyUseFeedback(float target, unsigned long nowMs,
                            unsigned long feedbackTimeoutMs);

  /** @brief Clears accumulated PI error without invalidating measurement. */
  void reset();

  /**
   * @brief Returns target corrected by this channel's feedback state.
   *
   * A zero target resets PI. An enabled channel with valid feedback remains
   * within its target direction, maxW, and the nonzero minW drive threshold.
   *
   * @param target Requested signed wheel speed.
   * @param minW Smallest nonzero usable target magnitude.
   * @param maxW Largest permitted target magnitude.
   * @param nowMs Current monotonic timestamp.
   * @return Corrected or passthrough signed target.
   */
  float correctTarget(float target, float minW, float maxW,
                      unsigned long nowMs);
};
