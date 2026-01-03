#include "FourWheelAngularSpeed.h"

FourWheelAngularSpeed::FourWheelAngularSpeed() : fl(0), fr(0), bl(0), br(0) {}

void FourWheelAngularSpeed::updateFrom(float fl, float fr, float bl, float br) {
  this->fl = fl;
  this->fr = fr;
  this->bl = bl;
  this->br = br;
}

float FourWheelAngularSpeed::getFlWInRad() const { return fl; }
float FourWheelAngularSpeed::getFrWInRad() const { return fr; }
float FourWheelAngularSpeed::getBlWInRad() const { return bl; }
float FourWheelAngularSpeed::getBrWInRad() const { return br; }

void FourWheelAngularSpeed::setFl(float w) { fl = w; }
void FourWheelAngularSpeed::setFr(float w) { fr = w; }
void FourWheelAngularSpeed::setBl(float w) { bl = w; }
void FourWheelAngularSpeed::setBr(float w) { br = w; }
