#include "IMUSensor.h"

IMUSensor::IMUSensor(
    OnUpdateIMUSensorEvent onUpdate,
    TwoWire *wire,
    uint8_t i2c_address,
    uint32_t rotationVectorIntervalinUs,
    uint32_t linearAccelerationIntervalinUs,
    uint32_t gyroscopeIntervalinUs)
{
    this->i2c_address = i2c_address;
    this->rotationVectorIntervalinUs = rotationVectorIntervalinUs;
    this->linearAccelerationIntervalinUs = linearAccelerationIntervalinUs;
    this->gyroscopeIntervalinUs = gyroscopeIntervalinUs;
    this->wire = wire;
    this->onUpdate = onUpdate;
    logger.debug("IMU Sensor - I2C Address: " + String(i2c_address) +
                 " - Rotation Vector Interval (us): " + String(rotationVectorIntervalinUs) +
                 " - Linear Acceleration Interval (us): " + String(linearAccelerationIntervalinUs) +
                 " - Gyroscope Interval (us): " + String(gyroscopeIntervalinUs));
}

IMUSensor *IMUSensor::begin()
{
    while (!init())
    {
        logger.error("Error initializing IMU sensor. Retrying in 500 ms...");
        delay(500);
    }
    logger.info("IMU sensor: Initialized.");
    return this;
}

bool IMUSensor::init()
{
    if (!sensor.begin_I2C(i2c_address, wire))
    {
        logger.error("Not found IMU sensor.");
        return false;
    }
    logger.info("IMU sensor found.");

    if (!sensor.enableReport(SH2_GAME_ROTATION_VECTOR, rotationVectorIntervalinUs))
    {
        logger.error("Cant enable rotation vector.");
        return false;
    }
    logger.info("IMU sensor: Rotation vector enable.");

    if (!sensor.enableReport(SH2_LINEAR_ACCELERATION, linearAccelerationIntervalinUs))
    {
        logger.error("Cant enable linear acceleration.");
        return false;
    }
    logger.info("IMU sensor: Linear acceleration enable.");

    if (!sensor.enableReport(SH2_GYROSCOPE_CALIBRATED, gyroscopeIntervalinUs))
    {
        logger.error("Cant enable gyroscope.");
        return false;
    }
    logger.info("IMU sensor: Gyroscope enable.");

    return true;
}

bool IMUSensor::update()
{
    if (sensor.getSensorEvent(&value))
    {
        logger.debug("Read IMU sensor value.");
        onUpdate(getValue());
        return true;
    }

    logger.debug("No new IMU sensor value.");
    return false;
}

IMUData *IMUSensor::getValue()
{
    if (value.sensorId == SH2_GAME_ROTATION_VECTOR)
    {
        imuData.setOrientation(
            value.un.rotationVector.real,
            value.un.rotationVector.i,
            value.un.rotationVector.j,
            value.un.rotationVector.k);
    }
    else if (value.sensorId == SH2_LINEAR_ACCELERATION)
    {
        imuData.setLinearAcceleration(
            value.un.linearAcceleration.x,
            value.un.linearAcceleration.y,
            value.un.linearAcceleration.z);
    }
    else if (value.sensorId == SH2_GYROSCOPE_CALIBRATED)
    {
        imuData.setAngularVelocity(
            value.un.gyroscope.x,
            value.un.gyroscope.y,
            value.un.gyroscope.z);
    }
    return &imuData;
}
