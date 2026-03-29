#pragma once
#include <Arduino.h>

#ifdef USE_ROS_LOGGER
#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <std_msgs/msg/string.h>
#endif

enum LogLevel { TRACE, DEBUG, INFO, WARN, ERROR, FATAL, OFF };

enum LogOutput {
    OUTPUT_SERIAL,
    OUTPUT_SERIAL2,
    OUTPUT_ROS
};

class Logger {
private:
    LogLevel  level  = INFO;
    LogOutput output = OUTPUT_SERIAL;
    unsigned long baud = 115200;
    bool initialized = false;

    void log(LogLevel level, String msg);
    void printToOutput(const String& prefix, const String& msg);

public:
    /**
     * @brief Constructor vacío — no hace nada.
     *        Llamar begin() en setup() para inicializar.
     */
    Logger() = default;

    /**
     * @brief Inicializa el logger con el destino elegido.
     *        Llamar en setup() antes de usar cualquier log.
     */
    void begin(unsigned long baud, LogLevel level = INFO, LogOutput output = OUTPUT_SERIAL);

    void setLevel(LogLevel level);
    void setOutput(LogOutput output);

    bool isDebug();
    bool isTrace();
    bool isInfo();
    bool isWarn();
    bool isError();
    bool isFatal();
    bool isOff();

    void debugPlot(String varName, float value);

    void trace(String msg);
    void debug(String msg);
    void info(String msg);
    void warn(String msg);
    void error(String msg);
    void fatal(String msg);

#ifdef USE_ROS_LOGGER
    void initRosPublisher(rcl_node_t* node, rclc_support_t* support);
#endif
};

extern Logger logger;
