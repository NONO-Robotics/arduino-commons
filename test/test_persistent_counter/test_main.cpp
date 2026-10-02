#include <unity.h>

#include "NativeTestCommon.h"

void test_persistent_counter_defaults_and_creates_file() {
  // Prepare
  resetMocks();
  PersistentCounter counter("/counter");

  // Perform

  // Asserts
  TEST_ASSERT_EQUAL_INT(0, counter.read());
  TEST_ASSERT_TRUE(LittleFS.exists("/counter"));
}

void test_persistent_counter_open_read_failure_returns_default() {
  // Prepare
  resetMocks();
  PersistentCounter counter("/counter");
  counter.read();
  LittleFS.open_read_result = false;

  // Perform

  // Asserts
  TEST_ASSERT_EQUAL_INT(0, counter.read());
}

void test_persistent_counter_malformed_content_returns_default() {
  // Prepare
  resetMocks();
  PersistentCounter counter("/counter");
  LittleFS.files["/counter"] = "malformed";

  // Perform

  // Asserts
  TEST_ASSERT_EQUAL_INT(0, counter.read());
}

void test_persistent_counter_write_failure_keeps_default() {
  // Prepare
  resetMocks();
  PersistentCounter counter("/counter");
  counter.read();
  LittleFS.open_write_result = false;

  // Perform
  counter.save(4);

  // Asserts
  TEST_ASSERT_EQUAL_INT(0, counter.read());
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_persistent_counter_defaults_and_creates_file);
  RUN_TEST(test_persistent_counter_open_read_failure_returns_default);
  RUN_TEST(test_persistent_counter_malformed_content_returns_default);
  RUN_TEST(test_persistent_counter_write_failure_keeps_default);
  return UNITY_END();
}
