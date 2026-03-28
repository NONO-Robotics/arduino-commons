#include "FourWheelBLDCController.h"

FourWheelBLDCController::FourWheelBLDCController(
    float maxW, int minPwm, int maxPwm, int pinPwmFrontRight,
    int pinDirFrontRight, int pinBrakeFrontRight, int pinPwmFrontLeft,
    int pinDirFrontLeft, int pinBrakeFrontLeft, int pinPwmBackRight,
    int pinDirBackRight, int pinBrakeBackRight, int pinPwmBackLeft,
    int pinDirBackLeft, int pinBrakeBackLeft,
    float frontFactor, float backFactor) {

  motorFrontRightController =
      (new BLDCMotorController(
           (new BLDCMotorBuilder(pinPwmFrontRight, pinDirFrontRight,
                                 pinBrakeFrontRight))
               ->setChannel(0)
                              ->invertDirection()
               ->build(),
           maxW, minPwm, maxPwm, frontFactor))
          ->setup();

  motorFrontLeftController =
      (new BLDCMotorController(
           (new BLDCMotorBuilder(pinPwmFrontLeft, pinDirFrontLeft,
                                 pinBrakeFrontLeft))
               ->setChannel(1)
               ->build(),
           maxW, minPwm, maxPwm, frontFactor))
          ->setup();

  motorBackRightController =
      (new BLDCMotorController(
           (new BLDCMotorBuilder(pinPwmBackRight, pinDirBackRight,
                                 pinBrakeBackRight))
               ->setChannel(3)
               ->invertDirection()
               ->build(),
           maxW, minPwm, maxPwm, backFactor))
          ->setup();

  motorBackLeftController =
      (new BLDCMotorController(
           (new BLDCMotorBuilder(pinPwmBackLeft, pinDirBackLeft,
                                 pinBrakeBackLeft))
               ->setChannel(4)
               ->build(),
           maxW, minPwm, maxPwm, backFactor))
          ->setup();
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
