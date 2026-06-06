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

  // Create the converter using the actual resolution of the motor
  // (If you configured the motor to 11 bits, this will pass 11 automatically)
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
  // 1. Convert float (rad/s) -> int (high resolution PWM)
  int targetPwm = wConverter->convert(radsBySeg);

  // 2. Send to the motor
  motor->setPwmSpeed(int(targetPwm * factor));
  return this;
};

// Direct PWM control (useful for testing)
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