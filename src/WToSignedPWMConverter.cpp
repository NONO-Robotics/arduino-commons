#include "WToSignedPWMConverter.h"

#include <math.h>

int intClamp(int valor, int min, int max) {
  if (valor < min)
    return min;
  if (valor > max)
    return max;
  return valor;
}

int floatSign(float value) { return value > 0 ? 1 : -1; }


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
  if (fabs(w) < 0.002)
    return 0;

  // Mapping calculation with floating point arithmetic for precision
  // PWM = ( |Current Omega| / Max Omega ) * maxPwmLimit_Counts
  int pwm = (fabs(w) / maxW) * (float)maxPwmLimit;

  pwm = intClamp(
    pwm, 
    minPwm, 
    maxPwm > 0 ? maxPwm: maxPwmLimit);

  return floatSign(w) * pwm;
};


