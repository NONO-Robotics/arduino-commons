#pragma once
#include <Arduino.h>

enum LogLevel {
  TRACE,
  DEBUG,
  INFO,
  WARN,
  ERROR,
  FATAL
};


class Logger {
private:
  LogLevel level = INFO; 

  void log(LogLevel level, String msg);

public:
  Logger(unsigned long baud, LogLevel level = INFO);

  bool isDebug();
  bool isTrace();
  bool isInfo();
  bool isWarn();
  bool isError();
  bool isFatal();

  void setLevel(LogLevel level);
  void debugPlot(String varName, float value);
  void trace(String msg);
  void debug(String msg);
  void info(String msg);
  void warn(String msg);
  void error(String msg);
  void fatal(String msg);
};

// Declare the logger instance as 'extern'
extern Logger logger; // <-- Add 'extern' here and remove initialization