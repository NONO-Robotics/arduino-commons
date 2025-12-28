#pragma once
#include "FS.h"
#include "LittleFS.h"
#include <ArduinoJson.h>

class ConfigStorage {
  private:
    String path;
    JsonDocument doc;

  public:
    ConfigStorage(String path = "/config.json");

    bool begin();

    String get(String key, String defaultValue = "");
    bool has(String key);
    void set(String key, String value);

    void save();
};