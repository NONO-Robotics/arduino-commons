#include <Arduino.h>
#include <functional>

/**
 * @brief Class to execute a function (closure/lambda) periodically
 * within the main loop, without using FreeRTOS tasks.
 */
class SimpleTimer {
public:
  using TaskFunction = std::function<void()>;

  /**
   * @brief Timer Constructor.
   * @param msInterval Interval execution time in milliseconds.
   * @param func Function (or lambda) to execute periodically.
   */
  SimpleTimer(uint32_t msInterval, TaskFunction func)
      : interval_ms(msInterval), taskFunc(func), last_execution_ms(0) {
    // Initializes last execution time on object creation.
  }

  /**
   * @brief Should be called in the main loop().
   * Checks if interval has passed and executes the function if so.
   */
  void update() {
    uint32_t current_ms = millis();

    // Handles millis() rollover correctly with subtraction.
    if (current_ms - last_execution_ms >= interval_ms) {

      // 1. Update last execution time BEFORE running the function.
      //    Ensures interval is based on real time, not execution duration.
      last_execution_ms = current_ms;

      // 2. Execute the closure/lambda
      if (taskFunc) {
        taskFunc();
      }
    }
  }

private:
  uint32_t interval_ms;
  TaskFunction taskFunc;
  uint32_t
      last_execution_ms; /// Stores the time of the last successful execution
};