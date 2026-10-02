#pragma once
#include "FS.h"
#include "LittleFS.h"
#include <ArduinoJson.h>
#include "Logger.h"

/**
 * @brief Loads, reads, and persists a JSON configuration document in LittleFS.
 */
class ConfigStorage {
private:
  String path;
  JsonDocument doc;

public:
  /**
   * @brief Selects the LittleFS path that stores the JSON configuration.
   * @param path LittleFS file path; defaults to "/config.json".
   */
  ConfigStorage(String path = "/config.json");

  /**
   * @brief Mounts LittleFS and loads the selected configuration file.
   * @return True when the filesystem and configuration load successfully.
   */
  bool begin();

  /**
   * @brief Looks up a string value in the loaded JSON document.
   * @param key JSON key to read.
   * @param defaultValue Value returned when the key is absent; defaults to an empty string.
   * @return Stored string value, or defaultValue when the key is absent.
   */
  String get(String key, String defaultValue = "");

  /**
   * @brief Tests whether the loaded JSON document contains a key.
   * @param key JSON key to test.
   * @return True when key exists in the document; otherwise false.
   */
  bool has(String key);

  /**
   * @brief Stores a string value under a JSON key in memory.
   * @param key JSON key to create or replace.
   * @param value String value to store.
   */
  void set(String key, String value);

  /**
   * @brief Serializes the in-memory configuration to its LittleFS file.
   */
  void save();
};
