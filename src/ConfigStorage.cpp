#include "ConfigStorage.h"

ConfigStorage::ConfigStorage(String path) :path(path) {}

bool ConfigStorage::begin()
{
    if (!LittleFS.begin(true))
        return false;
    if (LittleFS.exists(path))
    {
        File file = LittleFS.open(path, "r");
        if (file)
        {
            deserializeJson(doc, file);
            file.close();
        }
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
