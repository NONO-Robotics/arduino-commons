#pragma once

#include <HardwareSerial.h>
#include <GPSData.h>
#include <Logger.h>

using OnUpdateGpsSensorEvent = void(*)(GPSData *);

class GPSSensor
{
private:
    HardwareSerial *serial;
    GPSData data;
    OnUpdateGpsSensorEvent onUpdate;

public:
    GPSSensor(
        HardwareSerial *serial,
        int rxPin,
        int txPin,
        OnUpdateGpsSensorEvent onUpdate,
        int baudRate);

    void update();
};


class GPSSensorBuilder
{
private:
    // Core object, required by the constructor
    HardwareSerial *serial;

    // Required parameters, initialized to an "unset" state
    int rxPin = -1;
    int txPin = -1;
    OnUpdateGpsSensorEvent onUpdate = nullptr;

    // Optional parameter with a default value
    int baudRate = 9600;

public:
    /**
     * @brief Builder constructor requires only the serial object.
     * @param serial Pointer to the HardwareSerial instance (e.g., &Serial2).
     */
    GPSSensorBuilder(HardwareSerial *serial);

    /**
     * @brief REQUIRED: Set the RX and TX pins.
     */
    GPSSensorBuilder *setPins(int rx, int tx);

    /**
     * @brief REQUIRED: Register the on-update callback.
     */
    GPSSensorBuilder *setOnUpdateEvent(OnUpdateGpsSensorEvent onUpdate);

    /**
     * @brief OPTIONAL: Set the baud rate.
     */
    GPSSensorBuilder *setBaudRate(int rate);

    /**
     * @brief Build and return the final GPSSensor object.
     * @throws std::runtime_error or halts if required parameters are not set.
     */
    GPSSensor *build();
};