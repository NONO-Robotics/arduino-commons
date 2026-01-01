#pragma once
#include <BLDCMotor.h>
#include <WToSignedPWMConverter.h>

class BLDCMotorController {
  private:
    WToSignedPWMConverter *wConverter;
    BLDCMotor *motor;
    
  public:
    BLDCMotorController(
      BLDCMotor *motor,
      float maxW,
      int minPwm,
      int maxPwm);

    BLDCMotorController* setup();

    BLDCMotorController* setRadsBySegSpeed(float radsBySeg);

    // Control directo por PWM (útil para pruebas)
    BLDCMotorController* setPwmSpeed(int value);

    BLDCMotorController* stop();
};