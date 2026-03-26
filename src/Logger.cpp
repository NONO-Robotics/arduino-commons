#include "Logger.h"

Logger logger(115200);

Logger::Logger(unsigned long baud, LogLevel level)
{
    Serial.begin(baud);
    setLevel(level); 
    while (!Serial && millis() < 5000)
        ;
}

void Logger::setLevel(LogLevel level) {
    this->level = level;
}

void Logger::log(LogLevel level, String msg) {
  if (level >= this->level) {
    switch (level) {
      case TRACE:
        Serial.print("[TRACE] ");
        Serial.println(msg);
        break;
      case DEBUG:
        Serial.print("[DEBUG] ");
        Serial.println(msg);
        break;
      case INFO:
        Serial.print("[INFO] ");
        Serial.println(msg);
        break;
      case WARN:
        Serial.print("[WARN] ");
        Serial.println(msg);
        break;
      case ERROR:
        Serial.print("[ERROR] ");
        Serial.println(msg);
        break;
      case FATAL:
        Serial.print("[FATAL] ");
        Serial.println(msg);
        break;
      case OFF:
        break;
    }
  }
}

// 4. Funciones auxiliares para mayor facilidad de uso
void Logger::trace(String msg) {
  log(TRACE, msg);
}

void Logger::debug(String msg) {
  log(DEBUG, msg);
}

void Logger::info(String msg) {
  log(INFO, msg);
}

void Logger::warn(String msg) {
  log(WARN, msg);
}

void Logger::error(String msg) {
  log(ERROR, msg);
}

void Logger::fatal(String msg) {
  log(FATAL, msg);
}

void Logger::debugPlot(String varName, float value) {
  if (isDebug()) {
    Serial.println(">" + varName + ":" + String(value)); 
  }
}

bool Logger::isDebug() {
    return level <= DEBUG;
}

bool Logger::isTrace() {
    return level <= TRACE;
}

bool Logger::isInfo() {
    return level <= INFO;
}

bool Logger::isWarn() {
    return level <= WARN;
}

bool Logger::isError() {
    return level <= ERROR;
}

bool Logger::isFatal() {
    return level <= FATAL;
}