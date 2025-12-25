#pragma once
#include <Arduino.h>
#include <Wire.h>

#define AS5600_DEFAULT_ADDR 0x36
#define AS5600_ERROR_VALUE 0xFFFF

/**
 * @brief Class for interacting with the AS5600 magnetic rotary sensor.
 *
 * This class handles I2C communication with the AS5600 sensor to read
 * angular positions.
 */
class AS5600Sensor {
private:
  int _value;
  int _address;
  TwoWire *_i2cPort;

public:
  /**
   * @brief Constructor for AS5600Sensor.
   * @param i2cPort Pointer to the I2C interface (default &Wire).
   * @param address I2C address of the sensor (default 0x36).
   */
  AS5600Sensor(TwoWire *i2cPort = &Wire, int address = AS5600_DEFAULT_ADDR);

  /**
   * @brief Initialize the sensor.
   * @return true if initialization successful (sensor detected), false
   * otherwise.
   */
  bool begin();

  /**
   * @brief Check if the last read was successful.
   * @return true if successful, false otherwise.
   */
  bool isSuccessful() const;

  /**
   * @brief Get the last read raw angle value.
   * @return Raw angle value (0-4095).
   */
  int getValue();

  /**
   * @brief Read the current angle from the sensor over I2C.
   * @return The raw angle value read.
   */
  int update();
};