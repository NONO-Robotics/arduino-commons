#pragma once

#include <Adafruit_BNO08x.h>
#include <Arduino.h>
#include <IMUData.h>
#include <Logger.h>

#define IMU_SENSOR_I2C_ADDRESS 0x4B // Default I2C address for BNO08x sensor.
#define IMU_SENSOR_ROTATION_VECTOR_INTERVAL_US                                 \
  500 // 200 Hz. If you need higher frequency, change it.
#define IMU_SENSOR_LINEAR_ACCELERATION_INTERVAL_US                             \
  500 // 200 Hz. If you need higher frequency, change it.
#define IMU_SENSOR_GYROSCOPE_INTERVAL_US                                       \
  500 // 200 Hz. If you need higher frequency, change it.

/**
 * @brief Evaluation callback for IMU data updates.
 * @param data Pointer to the new IMUData.
 */
using OnUpdateIMUSensorEvent = void (*)(IMUData *);

/**
 * @brief Configures a BNO08x IMU and publishes its latest motion reports.
 *
 * The sensor is read over I2C and invokes the supplied callback after a new
 * report has been processed.
 */
class IMUSensor {
private:
  Adafruit_BNO08x sensor;
  sh2_SensorValue_t value;
  IMUData imuData;
  uint8_t i2c_address;
  uint32_t rotationVectorIntervalinUs;
  uint32_t linearAccelerationIntervalinUs;
  uint32_t gyroscopeIntervalinUs;
  TwoWire *wire;
  OnUpdateIMUSensorEvent onUpdate;

public:
  /**
   * @brief Create an IMU reader with report intervals and an update callback.
   *
   * @param onUpdate Callback invoked after new IMU data is processed.
   * @param wire I2C interface connected to the sensor.
   * @param i2c_address Seven-bit I2C address of the sensor.
   * @param rotationVectorIntervalinUs Rotation-vector report interval in microseconds.
   * @param linearAccelerationIntervalinUs Interval for linear acceleration
   * report.
   * @param gyroscopeIntervalinUs Gyroscope report interval in microseconds.
   */
  IMUSensor(OnUpdateIMUSensorEvent onUpdate, TwoWire *wire = &Wire,
            uint8_t i2c_address = IMU_SENSOR_I2C_ADDRESS,
            uint32_t rotationVectorIntervalinUs =
                IMU_SENSOR_ROTATION_VECTOR_INTERVAL_US,
            uint32_t linearAccelerationIntervalinUs =
                IMU_SENSOR_LINEAR_ACCELERATION_INTERVAL_US,
            uint32_t gyroscopeIntervalinUs = IMU_SENSOR_GYROSCOPE_INTERVAL_US);

  /**
   * @brief Initialize the sensor and enable configured reports.
   * @return Pointer to this IMUSensor instance after setup.
   */
  IMUSensor *begin();

  /**
   * @brief Start I2C communication and verify that the sensor responds.
   * @return true when sensor initialization succeeds.
   */
  bool init();

  /**
   * @brief Process one available sensor report and update cached IMU data.
   * @return true when a new report was processed.
   */
  bool update();

  /**
   * @brief Return the most recently processed IMU sample.
   * @return Pointer to the internally stored IMU data.
   */
  IMUData *getValue();
};
