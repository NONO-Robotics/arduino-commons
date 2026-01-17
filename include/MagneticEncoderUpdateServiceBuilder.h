#pragma once

#include "MagneticEncoderUpdateService.h"
#include "Logger.h"

class MagneticEncoderUpdateServiceBuilder
{
private:
    MagneticEncoder **encoders;
    int counter;
    int size;
    int multiplexorAddress;

public:
    MagneticEncoderUpdateServiceBuilder(
        int size, 
        int multiplexorAddress = I2C_MUX_DEFAULT_ADDRESS)
    {
        this->encoders = new MagneticEncoder*[size];
        this->size = size;
        this->multiplexorAddress = multiplexorAddress;
        this->counter = 0;
    }

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

    MagneticEncoderUpdateService* build() {
        return new MagneticEncoderUpdateService(
            encoders,
            this->size,
            this->multiplexorAddress);
    }

    int getSize() { return size; }
};
