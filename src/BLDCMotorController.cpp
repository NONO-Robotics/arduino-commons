#include <BLDCMotorController.h>

BLDCMotorController::BLDCMotorController(
    BLDCMotor *motor,
    float maxW,
    int minPwm,
    int maxPwm,
    float factor)
{
  this->motor = motor;
  this->factor = factor;

  // Creamos el convertidor usando la resolución real del motor
  // (Si configuraste el motor a 11 bits, esto pasará 11 automáticamente)
  this->wConverter = new WToSignedPWMConverter(
      maxW,
      motor->getResolutionInBits(),
      minPwm,
      maxPwm);
};

BLDCMotorController *BLDCMotorController::setup()
{
  this->motor->setup();
  return this;
};

BLDCMotorController *BLDCMotorController::setRadsBySegSpeed(float radsBySeg)
{
  // 1. Convertimos float (rad/s) -> int (PWM de alta resolución)
  int targetPwm = wConverter->convert(radsBySeg);

  // 2. Enviamos al motor
  motor->setPwmSpeed(int(targetPwm * factor));
  return this;
};

// Control directo por PWM (útil para pruebas)
BLDCMotorController *BLDCMotorController::setPwmSpeed(int value)
{
  motor->setPwmSpeed(value);
  return this;
};

BLDCMotorController *BLDCMotorController::stop()
{
  motor->stop();
  return this;
};