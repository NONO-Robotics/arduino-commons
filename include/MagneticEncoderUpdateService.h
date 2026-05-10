#pragma once

#include "I2CMultiplexor.h"
#include "MagneticEncoder.h"
#include "Logger.h"

/**
 * @brief Service to manage and update multiple MagneticEncoders via an I2C multiplexor.
 *
 * This class abstracts the process of selecting the correct I2C channel
 * on a multiplexor and updating each connected magnetic encoder.
 */
class MagneticEncoderUpdateService
{
private:
    I2CMultiplexor *multiplexor;
    MagneticEncoder **encoders;
    int encodersCount;

public:
    /**
     * @brief Constructor for the update service.
     * @param encoders Array of pointers to MagneticEncoder instances.
     * @param encodersCount Number of encoders in the array.
     * @param address I2C address of the multiplexor (default I2C_MUX_DEFAULT_ADDRESS).
     */
    MagneticEncoderUpdateService(
        MagneticEncoder **encoders,
        int encodersCount,
        int address = I2C_MUX_DEFAULT_ADDRESS)
    {
        this->multiplexor = new I2CMultiplexor(address);
        this->encoders = encoders;
        this->encodersCount = encodersCount;
    }

    /**
     * @brief Initializes the service and all registered encoders.
     * Starts the encoders and logs the initialization.
     */
    void begin()
    {
        for (int i = 0; i < this->encodersCount; i++){
            MagneticEncoder *encoder = encoders[i];
            encoder->begin();
            logger.info("Encoder on channel #" + String(encoder->getChannel()) + " initialized...");
        }
    }

    /**
     * @brief Updates all registered encoders.
     * Selects the proper multiplexor channel and polls each encoder.
     */
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
