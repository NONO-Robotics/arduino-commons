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
                                                    unsigned int pointCount,
                                                    int maxPwm)
    : points_(points), pointCount_(pointCount), maxPwm_(maxPwm) {}

int TableSpeedToPwmConverter::convert(float targetW) const {
  if (!isfinite(targetW) || points_ == nullptr || pointCount_ == 0 ||
      fabsf(targetW) < 0.002F) {
    return 0;
  }

  const int sign = targetW < 0.0F ? -1 : 1;
  const float requestedW = fabsf(targetW);
  const Point& first = points_[0];
  const Point& last = points_[pointCount_ - 1];
  const int limit = maxPwm_ > 0 ? maxPwm_ : last.pwm;

  if (requestedW <= first.w) {
    return sign * (first.pwm > limit ? limit : first.pwm);
  }

  for (unsigned int index = 0; index + 1 < pointCount_; ++index) {
    const Point& lower = points_[index];
    const Point& upper = points_[index + 1];
    if (requestedW <= upper.w) {
      const float deltaW = upper.w - lower.w;
      if (deltaW <= 0.0F) {
        return sign * (lower.pwm > limit ? limit : lower.pwm);
      }

      const float fraction = (requestedW - lower.w) / deltaW;
      const float pwm = lower.pwm + fraction * (upper.pwm - lower.pwm);
      const int roundedPwm = static_cast<int>(lroundf(pwm));
      return sign * (roundedPwm > limit ? limit : roundedPwm);
    }
  }

  if (pointCount_ < 2) {
    return sign * (last.pwm > limit ? limit : last.pwm);
  }

  const Point& previous = points_[pointCount_ - 2];
  const float deltaW = last.w - previous.w;
  const float deltaPwm = static_cast<float>(last.pwm - previous.pwm);

  if (deltaW <= 0.0F || deltaPwm <= 0.0F) {
    return sign * (last.pwm > limit ? limit : last.pwm);
  }

  const float extrapolatedPwm =
      last.pwm + (requestedW - last.w) * (deltaPwm / deltaW);
  if (!isfinite(extrapolatedPwm) ||
      extrapolatedPwm >= static_cast<float>(limit)) {
    return sign * limit;
  }

  return sign * static_cast<int>(lroundf(extrapolatedPwm));
}
