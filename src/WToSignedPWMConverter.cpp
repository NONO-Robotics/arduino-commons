#include "WToSignedPWMConverter.h"

WToSignedPWMConverter::WToSignedPWMConverter(
                                             float maxW,
                                             int pwmResolutionInBits,
                                             int minPwm,
                                             int maxPwm) {
  this->maxW = maxW;
  this->maxPwmLimit = (int)((1UL << pwmResolutionInBits) - 1);
  this->maxPwm = maxPwm;
  this->minPwm = minPwm;
};

int WToSignedPWMConverter::convert(float w) {
  // Absolute software deadzone (zero noise)
  if (abs(w) < 0.01)
    return 0;

  // Mapping calculation with floating point arithmetic for precision
  // PWM = ( |Current Omega| / Max Omega ) * maxPwmLimit_Counts
  int pwm = (abs(w) / maxW) * (float)maxPwmLimit;

  pwm = intClamp(
    pwm, 
    minPwm, 
    maxPwm > 0 ? maxPwm: maxPwmLimit);

  return floatSign(w) * pwm;
};


