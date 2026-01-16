#include "DeltaTimeComputer.h"

DeltaTimeComputer::DeltaTimeComputer(unsigned int delta)
{
  this->delta = delta;
}

DeltaTimeComputer *DeltaTimeComputer::setup()
{
  this->update();
  this->reset();
  return this;
}

void DeltaTimeComputer::update()
{
  this->currentTime = millis();
}

void DeltaTimeComputer::reset()
{
  this->lastTime = this->currentTime;
}

unsigned int DeltaTimeComputer::deltaInMillis()
{
  return this->currentTime - this->lastTime;
}

bool DeltaTimeComputer::hasBeenReached()
{
  return this->hasBeenReached(this->delta);
}

bool DeltaTimeComputer::hasBeenReached(unsigned int deltaParam)
{
  return this->deltaInMillis() >= deltaParam;
}

unsigned int DeltaTimeComputer::getDelta()
{
  return this->delta;
}

void DeltaTimeComputer::update(OnDeltaTimeReachedEvent event)
{
  if (this->hasBeenReached())
  {
    if (event != nullptr)
    {
      event();
    }
    this->reset();
  }
  this->update();
}