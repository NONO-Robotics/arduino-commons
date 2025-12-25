#include "MagneticEncoderBuilder.h"

MagneticEncoderBuilder::MagneticEncoderBuilder() {
  _callback = nullptr;
  _channel = 0;
  _sampleIntervalMs = DEFAULT_SAMPLE_INTERVAL_MS;
  _alpha = DEFAULT_ALPHA;
  _applyFilter = true;
  _deadZone = DEFAULT_DEAD_ZONE;
  _address = AS5600_DEFAULT_ADDR;
  _i2cPort = &Wire;
}

MagneticEncoderBuilder &MagneticEncoderBuilder::setCallback(OnUpdateWEvent cb) {
  _callback = cb;
  return *this;
}

MagneticEncoderBuilder &MagneticEncoderBuilder::setChannel(short int channel) {
  _channel = channel;
  return *this;
}

MagneticEncoderBuilder &
MagneticEncoderBuilder::setSampleInterval(unsigned long ms) {
  _sampleIntervalMs = ms;
  return *this;
}

MagneticEncoderBuilder &MagneticEncoderBuilder::setAlpha(double alpha) {
  _alpha = alpha;
  return *this;
}

MagneticEncoderBuilder &MagneticEncoderBuilder::withFilter(bool enable) {
  _applyFilter = enable;
  return *this;
}

MagneticEncoderBuilder &MagneticEncoderBuilder::setDeadZone(float deadZone) {
  _deadZone = deadZone;
  return *this;
}

MagneticEncoderBuilder &MagneticEncoderBuilder::setI2CAddress(int address) {
  _address = address;
  return *this;
}

MagneticEncoderBuilder &MagneticEncoderBuilder::setI2CPort(TwoWire *i2cPort) {
  _i2cPort = i2cPort;
  return *this;
}

MagneticEncoder *MagneticEncoderBuilder::build() {
  // Verify that the callback is not null, as it is mandatory in your logic
  // If it is null, you could throw an error or assign an empty function if you
  // prefer not to crash.
  if (_callback == nullptr) {
    // Option: Assign an empty callback or let the user handle the risk
    // Serial.println("Error: Callback not defined in MagneticEncoderBuilder");
  }

  return new MagneticEncoder(_callback, _channel, _sampleIntervalMs, _alpha,
                             _applyFilter, _deadZone, _address, _i2cPort);
}