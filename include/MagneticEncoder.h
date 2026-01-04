#pragma once
#include "AS5600Sensor.h"
#include <AS5600Sensor.h>
#include <Arduino.h>
#include <EncoderAngularVelocityEstimator.h>
#include <Logger.h>
#include <math.h>

/**
 * @brief Callback function type for velocity updates.
 * @param channel Motor channel.
 * @param step Current step count.
 * @param w Angular velocity in rad/s.
 */
typedef void (*OnUpdateWEvent)(short int channel, int step, float w);

const int DEFAULT_SAMPLE_INTERVAL_MS = 50;

/**
 * @brief Class for magnetic encoder based on AS5600 sensor.
 * Calculates angular velocity (W) from sensor readings.
 */
class MagneticEncoder {

private:
  bool applyFilter;
  short int channel;
  AS5600Sensor *sensor;
  uint16_t previousStep;              ///< Stores val of previous reading angle
  unsigned long previousUpdateTimeMs; ///< Stores time in ms of previous reading
  unsigned long sampleIntervalMs; ///< Desired sampling interval in milliseconds

  OnUpdateWEvent
      onUpdateEvent; ///< Function to call when angular velocity changes

  EncoderAngularVelocityEstimator *wEstimator;
  float currentW;
  uint16_t currentStep;

public:
  /**
   * @brief Constructor for MagneticEncoder.
   * @param cb Callback function for velocity updates.
   * @param channel Channel identifier.
   * @param sampleIntervalMs Sampling interval in ms.
   * @param alpha Filter smoothing factor.
   * @param applyFilter Enable/disable filtering.
   * @param deadZone Deadzone threshold.
   * @param address I2C address of sensor.
   * @param i2cPort I2C port.
   */
  MagneticEncoder(OnUpdateWEvent cb, short int channel = 0,
                  unsigned long sampleIntervalMs = DEFAULT_SAMPLE_INTERVAL_MS,
                  double alpha = DEFAULT_ALPHA, bool applyFilter = true,
                  float deadZone = DEFAULT_DEAD_ZONE,
                  int address = AS5600_DEFAULT_ADDR, TwoWire *i2cPort = &Wire);

  /**
   * @brief Initialize the encoder.
   * @return true if successful.
   */
  bool begin();

  /**
   * @brief Update the encoder state. Reads sensor and calculates W.
   */
  void update();

  /**
   * @brief Get the configured channel.
   * @return Channel ID.
   */
  short int getChannel();

  /**
   * @brief Get the current angular velocity.
   * @return Angular velocity in rad/s.
   */
  float getW();

  /**
   * @brief Get the current raw step value.
   * @return Raw step (0-4095).
   */
  uint16_t getStep();
};