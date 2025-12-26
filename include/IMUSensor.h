#pragma once

#include <Arduino.h>
#include <Adafruit_BNO08x.h>
#include <Logger.h>
#include <IMUData.h>

#define IMU_SENSOR_I2C_ADDRESS 0x4B                    // Default I2C address for BNO08x sensor.
#define IMU_SENSOR_ROTATION_VECTOR_INTERVAL_US 500     // 200 Hz. If you need higher frequency, change it.
#define IMU_SENSOR_LINEAR_ACCELERATION_INTERVAL_US 500 // 200 Hz. If you need higher frequency, change it.
#define IMU_SENSOR_GYROSCOPE_INTERVAL_US 500           // 200 Hz. If you need higher frequency, change it.

using OnUpdateIMUSensorEvent = void (*)(IMUData *);

class IMUSensor
{
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
    IMUSensor(
        OnUpdateIMUSensorEvent onUpdate,
        TwoWire *wire = &Wire,
        uint8_t i2c_address = IMU_SENSOR_I2C_ADDRESS,
        uint32_t rotationVectorIntervalinUs = IMU_SENSOR_ROTATION_VECTOR_INTERVAL_US,
        uint32_t linearAccelerationIntervalinUs = IMU_SENSOR_LINEAR_ACCELERATION_INTERVAL_US,
        uint32_t gyroscopeIntervalinUs = IMU_SENSOR_GYROSCOPE_INTERVAL_US);

    IMUSensor* begin();

    bool init();

    bool update();

    IMUData *getValue();
};
