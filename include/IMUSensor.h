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
   * @brief Construct a new IMUSensor object.
   *
   * @param onUpdate Callback for new IMU data.
   * @param wire Pointer to I2C interface.
   * @param i2c_address I2C address of the sensor.
   * @param rotationVectorIntervalinUs Interval for rotation vector report.
   * @param linearAccelerationIntervalinUs Interval for linear acceleration
   * report.
   * @param gyroscopeIntervalinUs Interval for gyroscope report.
   */
  IMUSensor(OnUpdateIMUSensorEvent onUpdate, TwoWire *wire = &Wire,
            uint8_t i2c_address = IMU_SENSOR_I2C_ADDRESS,
            uint32_t rotationVectorIntervalinUs =
                IMU_SENSOR_ROTATION_VECTOR_INTERVAL_US,
            uint32_t linearAccelerationIntervalinUs =
                IMU_SENSOR_LINEAR_ACCELERATION_INTERVAL_US,
            uint32_t gyroscopeIntervalinUs = IMU_SENSOR_GYROSCOPE_INTERVAL_US);

  /**
   * @brief Setup and start the sensor.
   * @return Pointer to this IMUSensor instance.
   */
  IMUSensor *begin();

  /**
   * @brief Initialize communications and checks sensor ID.
   * @return true if successful.
   */
  bool init();

  /**
   * @brief Update sensor readings.
   * @return true if new data was processed.
   */
  bool update();

  /**
   * @brief Get the latest IMU data.
   * @return Pointer to IMUData.
   */
  IMUData *getValue();
};
