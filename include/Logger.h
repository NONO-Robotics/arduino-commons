#pragma once
#include <Arduino.h>

/**
 * @brief Log levels for the Logger class.
 */
enum LogLevel { TRACE, DEBUG, INFO, WARN, ERROR, FATAL };

/**
 * @brief Simple logging utility with log levels.
 */
class Logger {
private:
  LogLevel level = INFO;

  void log(LogLevel level, String msg);

public:
  /**
   * @brief Construct a new Logger object.
   * @param baud Serial baud rate (only used if initialized manually, mostly
   * used for printing).
   * @param level Initial log level.
   */
  Logger(unsigned long baud, LogLevel level = INFO);

  bool isDebug();
  bool isTrace();
  bool isInfo();
  bool isWarn();
  bool isError();
  bool isFatal();

  /**
   * @brief Set the Log Level.
   * @param level New log level.
   */
  void setLevel(LogLevel level);

  /**
   * @brief Print a value for plotting (e.g. Serial Plotter).
   * @param varName Name of the variable.
   * @param value Value.
   */
  void debugPlot(String varName, float value);

  void trace(String msg);
  void debug(String msg);
  void info(String msg);
  void warn(String msg);
  void error(String msg);
  void fatal(String msg);
};

// Declare the logger instance as 'extern'
extern Logger logger;