#pragma once

#include "I2CMultiplexor.h"
#include "MagneticEncoder.h"
#include "Logger.h"

class MagneticEncoderUpdateService
{
private:
    I2CMultiplexor *multiplexor;
    MagneticEncoder **encoders;
    int encodersCount;

public:
    MagneticEncoderUpdateService(
        MagneticEncoder **encoders,
        int encodersCount,
        int address = I2C_MUX_DEFAULT_ADDRESS)
    {
        this->multiplexor = new I2CMultiplexor(address);
        this->encoders = encoders;
        this->encodersCount = encodersCount;
    }

    void begin()
    {
        for (int i = 0; i < this->encodersCount; i++){
            MagneticEncoder *encoder = encoders[i];
            encoder->begin();
            logger.info("Encoder on channel #" + String(encoder->getChannel()) + " initialized...");
        }
    }

    void update()
    {
        for (int i = 0; i < this->encodersCount; i++)
        {
            MagneticEncoder *encoder = encoders[i];
            multiplexor->selectChannel(encoder->getChannel());
            encoder->update();
        }
    }
};
