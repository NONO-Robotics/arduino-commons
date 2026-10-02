#pragma once

#include <Arduino.h>
#include <map>

class File {
 public:
  File() = default;
  explicit operator bool() const { return content_ != nullptr; }
  size_t print(const String& value) {
    if (!content_ || !writable_) return 0;
    *content_ += value;
    return value.size();
  }
  String readString() const { return content_ && readable_ ? *content_ : String{}; }
  void close() {}
 private:
  friend class FS;
  explicit File(String* content, bool readable = true, bool writable = true)
      : content_(content), readable_(readable), writable_(writable) {}
  String* content_ = nullptr;
  bool readable_ = true;
  bool writable_ = true;
};

class FS {
 public:
  bool begin(bool = false) { return begin_result; }
  bool exists(const char* path) const { return files.count(path) != 0; }
  bool remove(const char* path) { return files.erase(path) != 0; }
  bool format() { files.clear(); return true; }
  File open(const char* path, const char* mode = "r") {
    if (mode[0] == 'r') {
      if (!open_read_result || !exists(path)) return {};
      return File(&files[path], read_result, false);
    }
    if (!open_write_result) return {};
    return File(&files[path], false, write_result);
  }
  void reset() {
    begin_result = true;
    open_read_result = true;
    open_write_result = true;
    read_result = true;
    write_result = true;
    files.clear();
  }
  bool begin_result = true;
  bool open_read_result = true;
  bool open_write_result = true;
  bool read_result = true;
  bool write_result = true;
  std::map<String, String> files;
};
