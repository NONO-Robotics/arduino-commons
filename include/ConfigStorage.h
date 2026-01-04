#pragma once
#include "FS.h"
#include "LittleFS.h"
#include <ArduinoJson.h>

/**
 * @brief Helper class to store configuration in LittleFS (JSON format).
 */
class ConfigStorage {
private:
  String path;
  JsonDocument doc;

public:
  /**
   * @brief Construct a new Config Storage object.
   * @param path File path in LittleFS (default "/config.json").
   */
  ConfigStorage(String path = "/config.json");

  /**
   * @brief Mount LittleFS and load configuration file.
   * @return true if successful.
   */
  bool begin();

  /**
   * @brief Get a configuration value.
   * @param key JSON key.
   * @param defaultValue Value to return if key not found.
   * @return Value as String.
   */
  String get(String key, String defaultValue = "");

  /**
   * @brief Check if key exists.
   * @param key JSON key.
   * @return true if exists.
   */
  bool has(String key);

  /**
   * @brief Set a configuration value.
   * @param key JSON key.
   * @param value Value to store.
   */
  void set(String key, String value);

  /**
   * @brief Persist configuration to file.
   */
  void save();
};