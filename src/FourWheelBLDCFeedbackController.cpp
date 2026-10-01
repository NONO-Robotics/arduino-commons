#include "FourWheelBLDCFeedbackController.h"

#include <Arduino.h>

FourWheelBLDCFeedbackController::FourWheelBLDCFeedbackController(
    float maxW, int minPwm, int maxPwm, int pinPwmFrontRight,
    int pinDirFrontRight, int pinBrakeFrontRight, int pinPwmFrontLeft,
    int pinDirFrontLeft, int pinBrakeFrontLeft, int pinPwmBackRight,
    int pinDirBackRight, int pinBrakeBackRight, int pinPwmBackLeft,
    int pinDirBackLeft, int pinBrakeBackLeft, float frontFactor,
    float backFactor, float proportionalGain, float integralGain,
    float maxCorrection, float maxIntegral)
    : controller({pinPwmFrontRight, pinDirFrontRight, pinBrakeFrontRight, 0,
                   true, maxW, minPwm, maxPwm, frontFactor},
                  {pinPwmFrontLeft, pinDirFrontLeft, pinBrakeFrontLeft, 1,
                   false, maxW, minPwm, maxPwm, frontFactor},
                  {pinPwmBackRight, pinDirBackRight, pinBrakeBackRight, 3,
                   true, maxW, minPwm, maxPwm, backFactor},
                  {pinPwmBackLeft, pinDirBackLeft, pinBrakeBackLeft, 4,
                   false, maxW, minPwm, maxPwm, backFactor}),
       frontLeftController(proportionalGain, integralGain, maxCorrection,
                           maxIntegral),
       maxW(maxW < 0.0f ? -maxW : maxW), frontLeftMeasuredSpeed(0.0f),
       hasFrontLeftFeedback(false), frontLeftLastUpdateMs(0),
       hasFrontLeftUpdateTime(false) {}

void FourWheelBLDCFeedbackController::setFrontLeftFeedback(
    float measuredSpeed) {
  frontLeftMeasuredSpeed = measuredSpeed;
  hasFrontLeftFeedback = true;
}

void FourWheelBLDCFeedbackController::invalidateFrontLeftFeedback() {
  hasFrontLeftFeedback = false;
  frontLeftController.reset();
  hasFrontLeftUpdateTime = false;
}

void FourWheelBLDCFeedbackController::stop() {
  controller.stop();
  frontLeftController.reset();
  hasFrontLeftUpdateTime = false;
}

void FourWheelBLDCFeedbackController::applySpeed(
    const FourWheelAngularSpeed &fwAngularSpeed) {
  FourWheelAngularSpeed adjustedSpeed = fwAngularSpeed;

  const float target = fwAngularSpeed.getFlWInRad();
  if (target == 0.0f) {
    frontLeftController.reset();
    hasFrontLeftUpdateTime = false;
  } else if (hasFrontLeftFeedback) {
    const unsigned long now = millis();
    const float deltaSeconds = hasFrontLeftUpdateTime
                                   ? static_cast<float>(now - frontLeftLastUpdateMs) /
                                         1000.0f
                                   : 0.0f;
    frontLeftLastUpdateMs = now;
    hasFrontLeftUpdateTime = true;

    const float minimumCorrection =
        target > 0.0f ? -target : -maxW - target;
    const float maximumCorrection =
        target > 0.0f ? maxW - target : -target;
    const float correction = frontLeftController.update(
        target, frontLeftMeasuredSpeed, deltaSeconds, minimumCorrection,
        maximumCorrection);
    float correctedTarget = target + correction;
    if (target > 0.0f) {
      correctedTarget = correctedTarget < 0.0f ? 0.0f : correctedTarget;
      correctedTarget = correctedTarget > maxW ? maxW : correctedTarget;
    } else {
      correctedTarget = correctedTarget > 0.0f ? 0.0f : correctedTarget;
      correctedTarget = correctedTarget < -maxW ? -maxW : correctedTarget;
    }
    adjustedSpeed.setFlWInRad(correctedTarget);
  }

  controller.applySpeed(adjustedSpeed);
}
