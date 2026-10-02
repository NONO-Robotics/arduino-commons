#pragma once
#include <Arduino.h>

namespace logger_mock {
inline bool debug_enabled = false;
}

class NativeLogger {
 public:
  template <typename... Args> void info(Args&&...) {}
  template <typename... Args> void warning(Args&&...) {}
  template <typename... Args> void error(Args&&...) {}
  template <typename... Args> void debug(Args&&...) {}
  bool isDebug() const { return logger_mock::debug_enabled; }
};

inline NativeLogger logger;
