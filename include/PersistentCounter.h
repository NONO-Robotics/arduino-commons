#pragma once
#include <LittleFS.h>

/**
 * @brief Helper class to store a single counter value in LittleFS.
 */
class PersistentCounter {
private:
  const char *path;

public:
  /**
   * @brief Construct a new Persistent Counter.
   * @param path File path.
   */
  PersistentCounter(const char *path) { this->path = path; }

  /**
   * @brief Read the current counter value.
   * @return Current count or 0 if not found.
   */
  int read() {
    uint8_t currentCount = 0;

    if (LittleFS.exists(path)) {
      File f = LittleFS.open(path, "r");
      if (f) {
        currentCount = f.readString().toInt();
        f.close();
      }
    }
    return currentCount;
  }

  /**
   * @brief Save the counter value to file.
   * @param currentCount Value to save.
   */
  void save(uint8_t currentCount) {
    File f = LittleFS.open(path, "w");
    if (f) {
      f.print(currentCount);
      f.close();
    }
  }

  /**
   * @brief Delete the counter file (reset to 0).
   */
  void reset() {
    if (LittleFS.exists(path)) {
      LittleFS.remove(path);
    }
  }
};