#pragma once

#include "EncoderAngularVelocityEstimator.h"
#include "MagneticEncoder.h"

/**
 * @brief Collects optional MagneticEncoder settings before creating an encoder.
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
   * @brief Create a builder with the encoder's default configuration.
   */
  MagneticEncoderBuilder();

  /**
   * @brief Set the callback that receives calculated angular velocity updates.
   * @param cb Callback invoked after an angular-velocity update.
   * @return Reference to this builder for chaining.
   */
  MagneticEncoderBuilder &setCallback(OnUpdateWEvent cb);

  /**
   * @brief Select the I2C multiplexer channel used by the encoder.
   * @param channel I2C multiplexer channel identifier.
   * @return Reference to this builder for chaining.
   */
  MagneticEncoderBuilder &setChannel(short int channel);

  /**
   * @brief Set the minimum period between encoder sensor samples.
   * @param ms Interval in milliseconds.
   * @return Reference to this builder for chaining.
   */
  MagneticEncoderBuilder &setSampleInterval(unsigned long ms);

  /**
   * @brief Set the EWMA smoothing factor used when filtering velocity.
   * @param alpha Filter smoothing factor.
   * @return Reference to this builder for chaining.
   */
  MagneticEncoderBuilder &setAlpha(double alpha);

  /**
   * @brief Enable or disable velocity filtering.
   * @param enable true to apply the configured filter.
   * @return Reference to this builder for chaining.
   */
  MagneticEncoderBuilder &withFilter(bool enable);

  /**
   * @brief Set the angular-velocity threshold treated as zero.
   * @param deadZone Angular-velocity dead-zone threshold in rad/s.
   * @return Reference to this builder for chaining.
   */
  MagneticEncoderBuilder &setDeadZone(float deadZone);

  /**
   * @brief Set the I2C address used to access the AS5600 sensor.
   * @param address Seven-bit I2C address.
   * @return Reference to this builder for chaining.
   */
  MagneticEncoderBuilder &setI2CAddress(int address);

  /**
   * @brief Set the I2C bus used to communicate with the AS5600 sensor.
   * @param i2cPort Pointer to the selected I2C interface.
   * @return Reference to this builder for chaining.
   */
  MagneticEncoderBuilder &setI2CPort(TwoWire *i2cPort);

  /**
   * @brief Create an encoder using the accumulated configuration.
   * @return Pointer to the newly created magnetic encoder.
   */
  MagneticEncoder *build();
};
