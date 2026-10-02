#include "WheelSpeedFeedbackController.h"

WheelSpeedFeedbackController::WheelSpeedFeedbackController(
    const WheelFeedbackSettings &settings)
    : controller_(settings.proportionalGain, settings.integralGain,
                  settings.maxCorrection, settings.maxIntegral),
      measuredSpeed_(0.0f), hasFeedback_(false), enabled_(settings.enabled),
      lastFeedbackMs_(0), lastControlMs_(0), hasUpdateTime_(false),
      needsFreshCommand_(true), needsFreshWheelSpeeds_(true) {}

void WheelSpeedFeedbackController::receiveFreshCommand() {
  needsFreshCommand_ = false;
}

void WheelSpeedFeedbackController::updateFeedback(float measuredSpeed,
                                                  unsigned long timestampMs) {
  measuredSpeed_ = measuredSpeed;
  hasFeedback_ = true;
  lastFeedbackMs_ = timestampMs;
  needsFreshWheelSpeeds_ = false;
}

void WheelSpeedFeedbackController::fault() {
  hasFeedback_ = false;
  needsFreshCommand_ = true;
  needsFreshWheelSpeeds_ = true;
  reset();
}

bool WheelSpeedFeedbackController::canSafelyUseFeedback(
    float target, unsigned long nowMs, unsigned long feedbackTimeoutMs) {
  if (target == 0.0f || !enabled_) {
    return true;
  }
  if (!needsFreshCommand_ && !needsFreshWheelSpeeds_ && hasFeedback_ &&
      nowMs - lastFeedbackMs_ <= feedbackTimeoutMs) {
    return true;
  }

  if (!needsFreshCommand_ && !needsFreshWheelSpeeds_) {
    fault();
  }
  return false;
}

void WheelSpeedFeedbackController::reset() {
  controller_.reset();
  hasUpdateTime_ = false;
}

float WheelSpeedFeedbackController::correctTarget(float target, float minW,
                                                    float maxW,
                                                    unsigned long nowMs) {
  if (target == 0.0f) {
    reset();
    return 0.0f;
  }
  if (!enabled_ || !hasFeedback_) {
    return target;
  }

  const float deltaSeconds = hasUpdateTime_
                                  ? static_cast<float>(nowMs - lastControlMs_) / 1000.0f
                                  : 0.0f;
  lastControlMs_ = nowMs;
  hasUpdateTime_ = true;
  float corrected =
      target + controller_.update(target, measuredSpeed_, deltaSeconds);
  if (target > 0.0f) {
    corrected = corrected > maxW ? maxW : corrected;
    return corrected < minW ? minW : corrected;
  }

  corrected = corrected < -maxW ? -maxW : corrected;
  return corrected > -minW ? -minW : corrected;
}
