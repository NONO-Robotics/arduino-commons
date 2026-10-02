#pragma once

#include <GPSData.h>
#include <HardwareSerial.h>
#include <Logger.h>

using OnUpdateGpsSensorEvent = void (*)(GPSData *);

/**
 * @brief wrapper for GPS module handling (e.g. NEO-6M).
 *
 * Handles reading from a HardwareSerial and parsing GPS data.
 */
class GPSSensor {
private:
  HardwareSerial *serial;
  GPSData data;
  OnUpdateGpsSensorEvent onUpdate;

public:
  /**
   * @brief Construct a new GPSSensor object.
   *
   * @param serial Pointer to HardwareSerial used for GPS communication.
   * @param rxPin RX pin number.
   * @param txPin TX pin number.
   * @param onUpdate Callback function to call when new data is available.
   * @param baudRate Baud rate for serial communication.
   */
  GPSSensor(HardwareSerial *serial, int rxPin, int txPin,
            OnUpdateGpsSensorEvent onUpdate, int baudRate);

  /**
   * @brief Polls the GPS module for new data.
   *
   * Should be called frequently in the main loop.
   */
  void update();
};

/**
 * @brief Configures and creates a GPSSensor using a HardwareSerial port.
 *
 * Required serial pins and update callback are collected before build() creates
 * the sensor.
 */
class GPSSensorBuilder {
private:
  // Core object, required by the constructor
  HardwareSerial *serial;

  // Required parameters, initialized to an "unset" state
  int rxPin = -1;
  int txPin = -1;
  OnUpdateGpsSensorEvent onUpdate = nullptr;

  // Optional parameter with a default value
  int baudRate = 9600;

public:
  /**
   * @brief Builder constructor requires only the serial object.
   * @param serial Pointer to the HardwareSerial instance (e.g., &Serial2).
   */
  GPSSensorBuilder(HardwareSerial *serial);

  /**
   * @brief Set serial receive and transmit pins required by the GPS module.
   * @param rx GPIO number connected to the module's transmit pin.
   * @param tx GPIO number connected to the module's receive pin.
   * @return Pointer to this builder for chaining.
   */
  GPSSensorBuilder *setPins(int rx, int tx);

  /**
   * @brief Register the callback invoked when parsed GPS data is available.
   * @param onUpdate Callback receiving the latest GPS data.
   * @return Pointer to this builder for chaining.
   */
  GPSSensorBuilder *setOnUpdateEvent(OnUpdateGpsSensorEvent onUpdate);

  /**
   * @brief Set the serial data rate used to communicate with the GPS module.
   * @param rate Serial rate in bits per second.
   * @return Pointer to this builder for chaining.
   */
  GPSSensorBuilder *setBaudRate(int rate);

  /**
   * @brief Create a sensor from the configured serial connection and callback.
   * @throws std::runtime_error or halts if required parameters are not set.
   * @return Pointer to the newly created GPS sensor.
   */
  GPSSensor *build();
};
