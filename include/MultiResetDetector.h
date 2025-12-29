#pragma once
#include <LittleFS.h>
#include "Logger.h"

class MultiResetDetector
{
private:
    const char *_path = "/drd.txt";
    uint32_t _windowMs;
    uint8_t _targetResets; // Cantidad de resets configurables
    bool _cleared = false;
    bool _initialized = false;

public:
    // Ahora recibe el tiempo de ventana y la cantidad de resets deseada
    MultiResetDetector(uint32_t windowMs = 10000, uint8_t targetResets = 2)
        : _windowMs(windowMs), _targetResets(targetResets) {}

    bool detect()
    {
        if (!LittleFS.begin(true))
        {
            logger.error("[MultiResetDetector] Error: cant initialize LittleFS");
            return false;
        }
        _initialized = true;

        uint8_t currentCount = 0;

        // 1. Leer el contador actual si existe
        if (LittleFS.exists(_path))
        {
            File f = LittleFS.open(_path, "r");
            if (f)
            {
                currentCount = f.readString().toInt();
                f.close();
            }
        }

        currentCount++;
        logger.info("[MultiResetDetector]: Reset detected (" + String(currentCount) + "/" + String(_targetResets) + ")");

        // 2. Verificar si llegamos al objetivo
        if (currentCount >= _targetResets)
        {
            logger.info("[MultiResetDetector]: Target resets reached!");
            stop(); // Limpiamos para que el siguiente reset sea un inicio fresco
            return true;
        }

        // 3. Guardar el nuevo valor del contador
        File f = LittleFS.open(_path, "w");
        if (f)
        {
            f.print(currentCount);
            f.close();
        }
        
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

        if (LittleFS.exists(_path))
        {
            LittleFS.remove(_path);
        }
        _cleared = true;
    }
};