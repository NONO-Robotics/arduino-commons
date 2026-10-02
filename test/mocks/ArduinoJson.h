#pragma once

#include <Arduino.h>
#include <FS.h>
#include <map>
#include <sstream>
#include <string>

class DeserializationError;

class JsonVariant {
 public:
  JsonVariant(std::map<std::string, String>* values = nullptr, std::string key = {})
      : values_(values), key_(key) {}

  template <typename T> operator T() const { return as<T>(); }
  template <typename T> T as() const {
    if (!values_) return {};
    const auto value = values_->find(key_);
    return value != values_->end() ? T(value->second) : T{};
  }
  bool isNull() const { return !values_ || values_->count(key_) == 0; }
  template <typename T> JsonVariant& operator=(const T& value) {
    if (values_) (*values_)[key_] = String(value);
    return *this;
  }

 private:
  std::map<std::string, String>* values_;
  std::string key_;
};

class JsonDocument {
 public:
  JsonVariant operator[](const char* key) { return JsonVariant(&values_, key); }
  JsonVariant operator[](const String& key) { return (*this)[key.c_str()]; }

 private:
  friend DeserializationError deserializeJson(JsonDocument&, File&);
  friend size_t serializeJson(const JsonDocument&, File&);
  std::map<std::string, String> values_;
};

namespace arduinojson_mock {
inline bool deserialize_failure = false;
inline void reset() { deserialize_failure = false; }
}

class DeserializationError {
 public:
  explicit DeserializationError(bool failed = false) : failed_(failed) {}
  explicit operator bool() const { return failed_; }
 private:
  bool failed_;
};

inline DeserializationError deserializeJson(JsonDocument& doc, File& file) {
  if (arduinojson_mock::deserialize_failure) return DeserializationError(true);
  std::istringstream input(file.readString().c_str());
  std::string key;
  std::string value;
  while (std::getline(input, key, '\t') && std::getline(input, value)) {
    doc.values_[key] = String(value.c_str());
  }
  return DeserializationError(false);
}

inline size_t serializeJson(const JsonDocument& doc, File& file) {
  size_t size = 0;
  for (const auto& [key, value] : doc.values_) {
    String line = String(key.c_str()) + "\t" + value + "\n";
    size += file.print(line);
  }
  return size;
}
