#include "FourWheelAngularSpeed.h"
#include <math.h>

FourWheelAngularSpeed::FourWheelAngularSpeed() : fl(0), fr(0), bl(0), br(0) {}

void FourWheelAngularSpeed::updateFrom(float fl, float fr, float bl, float br) {
  this->fl = fl;
  this->fr = fr;
  this->bl = bl;
  this->br = br;
}

bool FourWheelAngularSpeed::lessThan(float threshold) const {
  return fabs(fl) < threshold && fabs(fr) < threshold &&
         fabs(bl) < threshold && fabs(br) < threshold;
}

float FourWheelAngularSpeed::getAverageLeftWInRad() const {
  return (fl + bl) / 2.0F;
}

float FourWheelAngularSpeed::getAverageRightWInRad() const {
  return (fr + br) / 2.0F;
}

float FourWheelAngularSpeed::getFlWInRad() const { return fl; }
float FourWheelAngularSpeed::getFrWInRad() const { return fr; }
float FourWheelAngularSpeed::getBlWInRad() const { return bl; }
float FourWheelAngularSpeed::getBrWInRad() const { return br; }

float FourWheelAngularSpeed::setFlWInRad(float w) { fl = w; return fl;}
float FourWheelAngularSpeed::setFrWInRad(float w) { fr = w; return fr;}
float FourWheelAngularSpeed::setBlWInRad(float w) { bl = w; return bl;}
float FourWheelAngularSpeed::setBrWInRad(float w) { br = w; return br;}
