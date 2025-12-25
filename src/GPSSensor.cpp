#include <GPSSensor.h>

GPSSensor::GPSSensor(
    HardwareSerial *serial,
    int rxPin,
    int txPin,
    OnUpdateGpsSensorEvent onUpdate,
    int baudRate)
{
    this->serial = serial;
    this->onUpdate = onUpdate;
    serial->begin(baudRate, SERIAL_8N1, rxPin, txPin);
    logger.debug("GPS Sensor: Settings - Pins RX: " + String(rxPin) + " TX: " + String(txPin) + " - BaudRate: " + String(baudRate));
    logger.info("GPS Sensor: Initialized.");
}

void GPSSensor::update()
{
    String line = "";
    while (serial->available() > 0)
    {
        int v = serial->read();
        data.encode(v);

        if (logger.isDebug())
        {
            line = line + String(char(v));
        }
    }
    if (line.length() > 0)
        logger.debug(line);

    if (data.isLocationUpdated())
    {
        (*onUpdate)(&data);
    }
};

/**
 * @brief Builder constructor requires only the serial object.
 * @param serial Pointer to the HardwareSerial instance (e.g., &Serial2).
 */
GPSSensorBuilder::GPSSensorBuilder(HardwareSerial *serial) : serial(serial) {}

/**
 * @brief REQUIRED: Set the RX and TX pins.
 */
GPSSensorBuilder *GPSSensorBuilder::setPins(int rx, int tx)
{
    this->rxPin = rx;
    this->txPin = tx;
    return this;
}

/**
 * @brief REQUIRED: Register the on-update callback.
 */
GPSSensorBuilder *GPSSensorBuilder::setOnUpdateEvent(OnUpdateGpsSensorEvent onUpdate)
{
    this->onUpdate = onUpdate;
    return this;
}

/**
 * @brief OPTIONAL: Set the baud rate.
 */
GPSSensorBuilder *GPSSensorBuilder::setBaudRate(int rate)
{
    this->baudRate = rate;
    return this;
}

/**
 * @brief Build and return the final GPSSensor object.
 * @throws std::runtime_error or halts if required parameters are not set.
 */
GPSSensor *GPSSensorBuilder::build()
{
    // --- VALIDATION AT BUILD TIME ---
    if (rxPin == -1 || txPin == -1)
    {
        logger.error("FATAL ERROR: RX/TX pins must be set before building GPSSensor.");
        while (1)
            ; // Halt
    }
    if (!onUpdate)
    {
        logger.error("FATAL ERROR: onUpdate callback must be set before building GPSSensor.");
        while (1)
            ; // Halt
    }

    return new GPSSensor(serial, rxPin, txPin, onUpdate, baudRate);
}