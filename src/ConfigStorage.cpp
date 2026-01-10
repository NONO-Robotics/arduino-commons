#include "ConfigStorage.h"

ConfigStorage::ConfigStorage(String path) :path(path) {}

bool ConfigStorage::begin()
{
    if (!LittleFS.begin(true)) {
        logger.error("Can't mount LittleFS");
        return false;
    }

    if (LittleFS.exists(path)) {
        File file = LittleFS.open(path, "r");
        if (file) {
            DeserializationError error = deserializeJson(doc, file);
            file.close();
            if (error) logger.error("JSON deserialization error");
        }
    } else {
        save();
    }
    return true;
}

String ConfigStorage::get(String key, String defaultValue)
{
    if (has(key))
        return doc[key].as<String>();
    else
        return defaultValue;
}

void ConfigStorage::set(String key, String value)
{
    doc[key] = value;
}

void ConfigStorage::save()
{
    File file = LittleFS.open(path, "w");
    if (file)
    {
        serializeJson(doc, file);
        file.close();
    }
}

bool ConfigStorage::has(String key)
{
    return !doc[key].isNull();
}
