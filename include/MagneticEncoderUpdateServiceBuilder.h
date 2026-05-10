#pragma once

#include "MagneticEncoderUpdateService.h"
#include "Logger.h"

/**
 * @brief Builder for creating a MagneticEncoderUpdateService.
 *
 * Facilitates the configuration and instantiation of multiple magnetic encoders
 * and their management service.
 */
class MagneticEncoderUpdateServiceBuilder
{
private:
    MagneticEncoder **encoders;
    int counter;
    int size;
    int multiplexorAddress;

public:
    /**
     * @brief Constructor for the builder.
     * @param size Total number of encoders to manage.
     * @param multiplexorAddress I2C address of the multiplexor.
     */
    MagneticEncoderUpdateServiceBuilder(
        int size, 
        int multiplexorAddress = I2C_MUX_DEFAULT_ADDRESS)
    {
        this->encoders = new MagneticEncoder*[size];
        this->size = size;
        this->multiplexorAddress = multiplexorAddress;
        this->counter = 0;
    }

    /**
     * @brief Registers a new encoder with the service.
     * @param cb Callback function when the angular velocity updates.
     * @param channel Multiplexor channel for the encoder.
     * @param sampleIntervalMs Sampling interval in milliseconds.
     * @param alpha EWMA filter alpha value.
     * @param applyFilter Whether to apply the filter.
     * @param deadZone Velocity deadzone.
     * @param address I2C address of the sensor.
     * @param i2cPort I2C port interface.
     * @return Pointer to this builder instance.
     */
    MagneticEncoderUpdateServiceBuilder *addEncoder(
        OnUpdateWEvent cb,
        short int channel = 0,
        unsigned long sampleIntervalMs = DEFAULT_SAMPLE_INTERVAL_MS,
        double alpha = DEFAULT_ALPHA,
        bool applyFilter = true,
        float deadZone = DEFAULT_DEAD_ZONE,
        int address = AS5600_DEFAULT_ADDR,
        TwoWire *i2cPort = &Wire)
    {
        logger.info("Create encoder on channel #" + String(channel) + "...");
        this->encoders[counter] = new MagneticEncoder(
            cb,
            channel,
            sampleIntervalMs,
            alpha,
            applyFilter,
            deadZone,
            address,
            i2cPort);
        counter++;
        return this;
    }

    /**
     * @brief Builds the configured MagneticEncoderUpdateService.
     * @return Pointer to the newly created service.
     */
    MagneticEncoderUpdateService* build() {
        return new MagneticEncoderUpdateService(
            encoders,
            this->size,
            this->multiplexorAddress);
    }

    /**
     * @brief Gets the total number of encoders configured.
     * @return The size value.
     */
    int getSize() { return size; }
};
