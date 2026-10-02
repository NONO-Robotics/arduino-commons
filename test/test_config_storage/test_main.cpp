#include <unity.h>

#include "NativeTestCommon.h"

void test_config_storage_mount_failure() {
  // Prepare
  resetMocks();
  LittleFS.begin_result = false;
  ConfigStorage mountFailure("/config.json");

  // Perform

  // Asserts
  TEST_ASSERT_FALSE(mountFailure.begin());
}

void test_config_storage_malformed_json_fallback() {
  // Prepare
  resetMocks();
  LittleFS.files["/config.json"] = "malformed";
  arduinojson_mock::deserialize_failure = true;
  ConfigStorage malformed("/config.json");

  // Perform

  // Asserts
  TEST_ASSERT_TRUE(malformed.begin());
  TEST_ASSERT_FALSE(malformed.has("mode"));
  TEST_ASSERT_EQUAL_STRING("fallback",
                           malformed.get("mode", "fallback").c_str());
}

void test_config_storage_write_failure_keeps_set_value() {
  // Prepare
  resetMocks();
  ConfigStorage writeFailure("/config.json");
  writeFailure.begin();
  LittleFS.remove("/config.json");
  LittleFS.open_write_result = false;
  writeFailure.set("mode", "native");

  // Perform
  writeFailure.save();

  // Asserts
  TEST_ASSERT_EQUAL_STRING("native",
                           writeFailure.get("mode", "fallback").c_str());
  TEST_ASSERT_TRUE(writeFailure.has("mode"));
  TEST_ASSERT_FALSE(LittleFS.exists("/config.json"));
}

void test_config_storage_unreadable_file_skips_parse() {
  // Prepare
  resetMocks();
  LittleFS.files["/config.json"] = "";
  LittleFS.open_read_result = false;
  ConfigStorage unreadableFile("/config.json");

  // Perform

  // Asserts
  TEST_ASSERT_TRUE(unreadableFile.begin());
}

void test_config_storage_valid_parse_exposes_keys() {
  // Prepare
  resetMocks();
  LittleFS.files["/config.json"] = "mode\tnative\n";
  ConfigStorage parsed("/config.json");

  // Perform

  // Asserts
  TEST_ASSERT_TRUE(parsed.begin());
  TEST_ASSERT_TRUE(parsed.has("mode"));
  TEST_ASSERT_EQUAL_STRING("native", parsed.get("mode", "fallback").c_str());
  TEST_ASSERT_EQUAL_STRING("fallback", parsed.get("missing", "fallback").c_str());
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_config_storage_mount_failure);
  RUN_TEST(test_config_storage_malformed_json_fallback);
  RUN_TEST(test_config_storage_write_failure_keeps_set_value);
  RUN_TEST(test_config_storage_unreadable_file_skips_parse);
  RUN_TEST(test_config_storage_valid_parse_exposes_keys);
  return UNITY_END();
}
