#pragma once
#include <Arduino.h>
#include <Wire.h>

const int I2C_MUX_DEFAULT_ADDRESS = 0x70;

/**
 * @brief Control TCA9548A I2C Multiplexer.
 */
class I2CMultiplexor {

private:
  const int addess;

public:
  /**
   * @brief Construct a new I2CMultiplexor.
   * @param addess I2C address of the multiplexer (default 0x70).
   */
  I2CMultiplexor(int addess = I2C_MUX_DEFAULT_ADDRESS) : addess(addess) {}

  /**
   * @brief Select an I2C channel.
   *
   * Writes to the multiplexer to enable the specified channel.
   * @param channel Channel number (0-7).
   */
  void selectChannel(uint8_t channel);
};
