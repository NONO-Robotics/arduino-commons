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

int WToSignedPWMConverter::convert(float w) const {
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

TableSpeedToPwmConverter::TableSpeedToPwmConverter(const Point* points,
                                                   unsigned int pointCount)
    : points_(points), pointCount_(pointCount) {}

int TableSpeedToPwmConverter::convert(float targetW) const {
  if (!isfinite(targetW) || points_ == nullptr || pointCount_ == 0 ||
      fabsf(targetW) < 0.002F) {
    return 0;
  }

  const int sign = targetW < 0.0F ? -1 : 1;
  const float requestedW = fabsf(targetW);
  if (requestedW <= points_[0].w) {
    return sign * points_[0].pwm;
  }

  const Point& last = points_[pointCount_ - 1];
  if (requestedW >= last.w) {
    return sign * last.pwm;
  }

  for (unsigned int index = 0; index < pointCount_ - 1; ++index) {
    const Point& lower = points_[index];
    const Point& upper = points_[index + 1];
    if (requestedW <= upper.w) {
      const float fraction = (requestedW - lower.w) / (upper.w - lower.w);
      return sign * static_cast<int>(
                        lroundf(lower.pwm + fraction * (upper.pwm - lower.pwm)));
    }
  }

  return sign * last.pwm;
}
