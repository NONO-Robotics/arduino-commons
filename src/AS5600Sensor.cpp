#include "AS5600Sensor.h"

AS5600Sensor::AS5600Sensor(TwoWire *i2cPort, int address)
    : _i2cPort(i2cPort), _address(address), _value(AS5600_ERROR_VALUE) {}

bool AS5600Sensor::begin() {
  update();
  return isSuccessful();
}

bool AS5600Sensor::isSuccessful() const { return _value != AS5600_ERROR_VALUE; }

int AS5600Sensor::getValue() { return _value; }

int AS5600Sensor::update() {
  _i2cPort->beginTransmission(_address);
  _i2cPort->write(0x0E);                         // Angle register address (MSB)
  byte error = _i2cPort->endTransmission(false); // Do not release I2C bus

  if (error != 0) {
    _value = AS5600_ERROR_VALUE;
    return _value;
  }

  // Request 2 bytes from the sensor
  byte bytesReceived = _i2cPort->requestFrom(_address, 2);

  if (bytesReceived == 2) {
    // Read the two bytes and combine them into a 16-bit integer
    _value = _i2cPort->read() << 8; // MSB (high byte)
    _value |= _i2cPort->read();     // LSB (low byte)
  } else {
    // Error: Did not receive enough bytes
    _value = AS5600_ERROR_VALUE;
  }
  _i2cPort->endTransmission(_address);
  return _value;
}