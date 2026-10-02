#pragma once
#include <Arduino.h>
#include <Wire.h>

/** @brief Default I2C address assigned to an AS5600 sensor. */
#define AS5600_DEFAULT_ADDR 0x36
/** @brief Raw-angle sentinel returned when an I2C read fails. */
#define AS5600_ERROR_VALUE 0xFFFF

/**
 * @brief Reads raw angular positions from an AS5600 magnetic rotary sensor.
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
   * @brief Configures the I2C bus and address used to communicate with a sensor.
   * @param i2cPort I2C interface; defaults to the global Wire bus.
   * @param address Seven-bit I2C address; defaults to AS5600_DEFAULT_ADDR.
   */
  AS5600Sensor(TwoWire *i2cPort = &Wire, int address = AS5600_DEFAULT_ADDR);

  /**
   * @brief Verifies that the sensor responds on the configured I2C bus.
   * @return True when initialization succeeds; otherwise false.
   */
  bool begin();

  /**
   * @brief Reports whether the most recent sensor read succeeded.
   * @return True after a successful read; otherwise false.
   */
  bool isSuccessful() const;

  /**
   * @brief Returns the raw angle retained from the most recent read.
   * @return Raw AS5600 angle in encoder counts from 0 to 4095.
   */
  int getValue();

  /**
   * @brief Reads and stores the current raw angle through I2C.
   * @return Raw AS5600 angle in encoder counts, or AS5600_ERROR_VALUE on failure.
   */
  int update();
};
