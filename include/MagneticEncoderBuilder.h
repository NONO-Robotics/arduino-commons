#pragma once

#include "EncoderAngularVelocityEstimator.h"
#include "MagneticEncoder.h"

/**
 * @brief Builder class for MagneticEncoder.
 */
class MagneticEncoderBuilder {
private:
  OnUpdateWEvent _callback;
  short int _channel;
  unsigned long _sampleIntervalMs;
  double _alpha;
  bool _applyFilter;
  float _deadZone;
  int _address;
  TwoWire *_i2cPort;

public:
  /**
   * @brief Constructor.
   */
  MagneticEncoderBuilder();

  /**
   * @brief Set the callback function.
   * @param cb Callback function.
   * @return Builder instance.
   */
  MagneticEncoderBuilder &setCallback(OnUpdateWEvent cb);

  /**
   * @brief Set the channel ID.
   * @param channel Channel ID.
   * @return Builder instance.
   */
  MagneticEncoderBuilder &setChannel(short int channel);

  /**
   * @brief Set the sampling interval.
   * @param ms Interval in milliseconds.
   * @return Builder instance.
   */
  MagneticEncoderBuilder &setSampleInterval(unsigned long ms);

  /**
   * @brief Set the filter alpha.
   * @param alpha Alpha value.
   * @return Builder instance.
   */
  MagneticEncoderBuilder &setAlpha(double alpha);

  /**
   * @brief Enable or disable filter.
   * @param enable True to enable.
   * @return Builder instance.
   */
  MagneticEncoderBuilder &withFilter(bool enable);

  /**
   * @brief Set deadzone.
   * @param deadZone Deadzone value.
   * @return Builder instance.
   */
  MagneticEncoderBuilder &setDeadZone(float deadZone);

  /**
   * @brief Set I2C address.
   * @param address I2C address.
   * @return Builder instance.
   */
  MagneticEncoderBuilder &setI2CAddress(int address);

  /**
   * @brief Set I2C port.
   * @param i2cPort Pointer to TwoWire instance.
   * @return Builder instance.
   */
  MagneticEncoderBuilder &setI2CPort(TwoWire *i2cPort);

  /**
   * @brief Build the MagneticEncoder.
   * @return Pointer to new MagneticEncoder instance.
   */
  MagneticEncoder *build();
};