#include "BoundedPIController.h"

BoundedPIController::BoundedPIController(float proportionalGain,
                                         float integralGain,
                                         float maxCorrection,
                                         float maxIntegral)
    : proportionalGain(proportionalGain), integralGain(integralGain),
      maxCorrection(maxCorrection < 0.0f ? -maxCorrection : maxCorrection),
      maxIntegral(maxIntegral < 0.0f ? -maxIntegral : maxIntegral),
      integral(0.0f) {}

float BoundedPIController::clamp(float value, float minimum,
                                 float maximum) const {
  if (value > maximum) {
    return maximum;
  }
  if (value < minimum) {
    return minimum;
  }
  return value;
}

float BoundedPIController::update(float target, float measured,
                                  float deltaSeconds) {
  return update(target, measured, deltaSeconds, -maxCorrection,
                maxCorrection);
}

float BoundedPIController::update(float target, float measured,
                                  float deltaSeconds,
                                  float minimumCorrection,
                                  float maximumCorrection) {
  const float error = target - measured;
  const float elapsed = deltaSeconds > 0.0f ? deltaSeconds : 0.0f;
  const float proposedIntegral = clamp(integral + error * elapsed,
                                       -maxIntegral, maxIntegral);
  const float proportional = proportionalGain * error;
  const float unconstrained = proportional + integralGain * proposedIntegral;
  const float lowerBound = minimumCorrection > -maxCorrection
                               ? minimumCorrection
                               : -maxCorrection;
  const float upperBound = maximumCorrection < maxCorrection
                               ? maximumCorrection
                               : maxCorrection;

  if (lowerBound > upperBound) {
    return 0.0f;
  }

  const float correction = clamp(unconstrained, lowerBound, upperBound);

  if (unconstrained == correction ||
      (unconstrained > upperBound && error < 0.0f) ||
      (unconstrained < lowerBound && error > 0.0f)) {
    integral = proposedIntegral;
  }

  return correction;
}

void BoundedPIController::reset() { integral = 0.0f; }
