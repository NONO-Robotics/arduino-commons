#pragma once
#include <LittleFS.h>

class PersistentCounter
{
private:
    const char *path;

public:
    PersistentCounter(const char *path)
    {
        this->path = path;
    }

    int read()
    {
        uint8_t currentCount = 0;

        // 1. Leer el contador actual si existe
        if (LittleFS.exists(path))
        {
            File f = LittleFS.open(path, "r");
            if (f)
            {
                currentCount = f.readString().toInt();
                f.close();
            }
        }
        return currentCount;
    }

    void save(uint8_t currentCount)
    {
        // 3. Guardar el nuevo valor del contador
        File f = LittleFS.open(path, "w");
        if (f)
        {
            f.print(currentCount);
            f.close();
        }
    }

    void reset()
    {
        if (LittleFS.exists(path))
        {
            LittleFS.remove(path);
        }
    }
};