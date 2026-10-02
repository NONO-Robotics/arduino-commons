#include "OutdoorFourWheelFeedbackController.h"

#include <Arduino.h>

OutdoorFourWheelFeedbackController::OutdoorFourWheelFeedbackController(
    float minW, unsigned long feedbackTimeoutMs,
    const BLDCMotorSettings &frontRightMotor,
    const BLDCMotorSettings &frontLeftMotor,
    const BLDCMotorSettings &backRightMotor,
    const BLDCMotorSettings &backLeftMotor,
    const WheelFeedbackSettings &frontRight,
    const WheelFeedbackSettings &frontLeft,
    const WheelFeedbackSettings &backRight,
    const WheelFeedbackSettings &backLeft)
    : controller_(frontRightMotor, frontLeftMotor, backRightMotor, backLeftMotor),
      frontRight_(frontRight), frontLeft_(frontLeft), backRight_(backRight),
      backLeft_(backLeft), minW_(minW < 0.0f ? -minW : minW),
      maxW_(frontRightMotor.maxW < 0.0f ? -frontRightMotor.maxW
                                        : frontRightMotor.maxW),
      frontRightSpeedScale_(frontRight.speedScale),
      frontLeftSpeedScale_(frontLeft.speedScale),
      backRightSpeedScale_(backRight.speedScale),
      backLeftSpeedScale_(backLeft.speedScale),
      feedbackTimeoutMs_(feedbackTimeoutMs) {}

WheelSpeedFeedbackController &
OutdoorFourWheelFeedbackController::channel(WheelPosition wheel)
{
  switch (wheel)
  {
  case WheelPosition::FrontRight:
    return frontRight_;
  case WheelPosition::FrontLeft:
    return frontLeft_;
  case WheelPosition::BackRight:
    return backRight_;
  case WheelPosition::BackLeft:
    return backLeft_;
  }
  return frontLeft_;
}

float OutdoorFourWheelFeedbackController::targetFor(
    const FourWheelAngularSpeed &speed, WheelPosition wheel)
{
  switch (wheel)
  {
  case WheelPosition::FrontRight:
    return speed.getFrWInRad();
  case WheelPosition::FrontLeft:
    return speed.getFlWInRad();
  case WheelPosition::BackRight:
    return speed.getBrWInRad();
  case WheelPosition::BackLeft:
    return speed.getBlWInRad();
  }
  return 0.0f;
}

void OutdoorFourWheelFeedbackController::setTarget(
    FourWheelAngularSpeed &speed, WheelPosition wheel, float target)
{
  switch (wheel)
  {
  case WheelPosition::FrontRight:
    speed.setFrWInRad(target);
    return;
  case WheelPosition::FrontLeft:
    speed.setFlWInRad(target);
    return;
  case WheelPosition::BackRight:
    speed.setBrWInRad(target);
    return;
  case WheelPosition::BackLeft:
    speed.setBlWInRad(target);
    return;
  }
}

void OutdoorFourWheelFeedbackController::receiveFreshCommand()
{
  frontRight_.receiveFreshCommand();
  frontLeft_.receiveFreshCommand();
  backRight_.receiveFreshCommand();
  backLeft_.receiveFreshCommand();
}

void OutdoorFourWheelFeedbackController::updateFeedback(
    const WheelSpeeds &wheelSpeeds)
{
  const unsigned long timestampMs = millis();
  frontLeft_.updateFeedback(wheelSpeeds.getFlWInRad() /
                                      frontLeftSpeedScale_,
                                  timestampMs);
  frontRight_.updateFeedback(wheelSpeeds.getFrWInRad() /
                                       frontRightSpeedScale_,
                                   timestampMs);
  backLeft_.updateFeedback(wheelSpeeds.getBlWInRad() /
                                     backLeftSpeedScale_,
                                 timestampMs);
  backRight_.updateFeedback(wheelSpeeds.getBrWInRad() /
                                      backRightSpeedScale_,
                                  timestampMs);
}

void OutdoorFourWheelFeedbackController::faultFeedback(WheelPosition wheel)
{
  channel(wheel).fault();
}

void OutdoorFourWheelFeedbackController::faultAllFeedback()
{
  frontRight_.fault();
  frontLeft_.fault();
  backRight_.fault();
  backLeft_.fault();
}

void OutdoorFourWheelFeedbackController::stop()
{
  controller_.stop();
  frontRight_.reset();
  frontLeft_.reset();
  backRight_.reset();
  backLeft_.reset();
}

bool OutdoorFourWheelFeedbackController::applySpeed(
    const FourWheelAngularSpeed &fwAngularSpeed)
{
  const unsigned long now = millis();

  FourWheelAngularSpeed corrected = fwAngularSpeed;

  const WheelPosition wheels[] = {WheelPosition::FrontRight,
                                  WheelPosition::FrontLeft,
                                  WheelPosition::BackRight,
                                  WheelPosition::BackLeft};

  for (WheelPosition wheel : wheels)
  {
    if (!channel(wheel).canSafelyUseFeedback(targetFor(fwAngularSpeed, wheel),
                                             now, feedbackTimeoutMs_))
    {
      stop();
      return false;
    }
    setTarget(corrected, wheel,
              channel(wheel).correctTarget(targetFor(fwAngularSpeed, wheel),
                                           minW_, maxW_, now));
  }
  controller_.applySpeed(corrected);
  return true;
}
