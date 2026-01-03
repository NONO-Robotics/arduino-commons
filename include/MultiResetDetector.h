#pragma once
#include "PersistentCounter.h"
#include "Logger.h"

class MultiResetDetector
{
private:
    uint32_t _windowMs;
    uint8_t _targetResets; // Cantidad de resets configurables
    bool _cleared = false;
    bool _initialized = false;
    PersistentCounter *counter;

public:
    // Ahora recibe el tiempo de ventana y la cantidad de resets deseada
    MultiResetDetector(uint32_t windowMs = 10000, uint8_t targetResets = 2)
        : _windowMs(windowMs), _targetResets(targetResets)
    {
        counter = new PersistentCounter("/drd.txt");
    }

    bool detect()
    {
        if (!LittleFS.begin(true))
        {
            logger.error("[MultiResetDetector] Error: cant initialize LittleFS");
            return false;
        }
        _initialized = true;

        uint8_t currentCount = counter->read();

        // Si el reinitio se dio por precionar el boton reset.
        if (esp_reset_reason() == ESP_RST_EXT)
        {
            currentCount++;
            logger.info("[MultiResetDetector]: Reset detected (" + String(currentCount) + "/" + String(_targetResets) + ")");
        }

        // Verificar si llegamos al objetivo
        if (currentCount >= _targetResets)
        {
            logger.info("[MultiResetDetector]: Target resets reached!");
            stop(); // Limpiamos para que el siguiente reset sea un inicio fresco
            return true;
        }

        counter->save(currentCount);

        return false;
    }

    void process()
    {
        if (!_initialized)
            return;

        // Si el robot sobrevive encendido más de _windowMs, reseteamos el contador a 0
        if (!_cleared && millis() > _windowMs)
        {
            stop();
            logger.info("[MultiResetDetector]: Reset window closed. Counter cleared.");
        }
    }

    void stop()
    {
        if (!_initialized)
            return;

        counter->reset();
        _cleared = true;
    }
};