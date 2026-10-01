#include "FourWheelBLDCController.h"

namespace {
BLDCMotorController *buildController(const BLDCMotorSettings &settings) {
  BLDCMotorBuilder *builder = new BLDCMotorBuilder(
      settings.pwmPin, settings.directionPin, settings.brakePin);
  builder->setChannel(settings.pwmChannel)->setResolutionInBits(8);
  if (settings.invertDirection) {
    builder->invertDirection();
  }
  return (new BLDCMotorController(builder->build(), settings.maxW,
                                  settings.minPwm, settings.maxPwm,
                                  settings.factor))
      ->setup();
}
}  // namespace

FourWheelBLDCController::FourWheelBLDCController(
    const BLDCMotorSettings &frontRight, const BLDCMotorSettings &frontLeft,
    const BLDCMotorSettings &backRight, const BLDCMotorSettings &backLeft) {
  motorFrontRightController = buildController(frontRight);
  motorFrontLeftController = buildController(frontLeft);
  motorBackRightController = buildController(backRight);
  motorBackLeftController = buildController(backLeft);
}

void FourWheelBLDCController::stop() {
  motorFrontRightController->stop();
  motorFrontLeftController->stop();
  motorBackRightController->stop();
  motorBackLeftController->stop();
}

void FourWheelBLDCController::applySpeed(
    const FourWheelAngularSpeed& fwAngularSpeed) {
  motorFrontRightController->setRadsBySegSpeed(fwAngularSpeed.getFrWInRad());
  motorFrontLeftController->setRadsBySegSpeed(fwAngularSpeed.getFlWInRad());
  motorBackRightController->setRadsBySegSpeed(fwAngularSpeed.getBrWInRad());
  motorBackLeftController->setRadsBySegSpeed(fwAngularSpeed.getBlWInRad());
}
