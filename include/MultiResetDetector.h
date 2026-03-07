#pragma once
#include "Logger.h"
#include "PersistentCounter.h"

/**
 * @brief Detects multiple consecutive resets within a time window.
 *
 * Useful for entering special modes (like configuration) if the user
 * resets the device multiple times quickly.
 */
class MultiResetDetector {
private:
  uint32_t _windowMs;
  uint8_t _targetResets;
  bool _cleared = false;
  bool _initialized = false;
  PersistentCounter *counter;

public:
  /**
   * @brief Construct a new Multi Reset Detector.
   *
   * @param windowMs Time window in milliseconds to clear the counter.
   * @param targetResets Number of resets required to trigger detection.
   */
  MultiResetDetector(uint32_t windowMs = 10000, uint8_t targetResets = 2)
      : _windowMs(windowMs), _targetResets(targetResets) {
    counter = nullptr;
  }

  /**
   * @brief Check if multiple resets happened.
   *
   * @return true if target resets reached.
   */
  bool detect() {
    if (!LittleFS.begin(false)) {
        LittleFS.format();
        if (!LittleFS.begin(false)) {
            logger.error("[MultiResetDetector] Error: cant initialize LittleFS");
            return false;
        }
        logger.error("[MultiResetDetector] LittleFS formated");
    }
    _initialized = true;

    if (counter == nullptr) {
        counter = new PersistentCounter("/drd.txt");
    }

    uint8_t currentCount = counter->read();

    // If reset was caused by external pin (RST button)
    if (esp_reset_reason() == ESP_RST_POWERON || 
        esp_reset_reason() == ESP_RST_SW) {
        currentCount++;
        logger.info("[MultiResetDetector]: Reset detected (" +
                    String(currentCount) + "/" + String(_targetResets) + ")");
    }

    // Check if target reached
    if (currentCount >= _targetResets) {
      logger.info("[MultiResetDetector]: Target resets reached!");
      stop(); // Clear so next boot is fresh
      return true;
    }

    counter->save(currentCount);

    return false;
  }

  /**
   * @brief Loop process to check timeout.
   *
   * Should be called in loop() to clear the counter after windowMs.
   */
  void process() {
    if (!_initialized)
      return;

    // If alive for more than _windowMs, reset counter
    if (!_cleared && millis() > _windowMs) {
      stop();
      logger.info(
          "[MultiResetDetector]: Reset window closed. Counter cleared.");
    }
  }

  /**
   * @brief Clear the reset counter immediately.
   */
  void stop() {
    if (!_initialized)
      return;

    counter->reset();
    _cleared = true;
  }
};