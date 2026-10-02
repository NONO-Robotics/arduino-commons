#include <unity.h>

#include "NativeTestCommon.h"

void test_multi_reset_detector_removes_marker_after_window() {
  // Prepare
  resetMocks();
  arduino_mock::reset_reason = ESP_RST_SW;
  MultiResetDetector belowThreshold(10, 2);

  // Perform

  // Asserts
  TEST_ASSERT_FALSE(belowThreshold.detect());
  arduino_mock::now_ms = 10;
  belowThreshold.process();
  TEST_ASSERT_TRUE(LittleFS.exists("/drd.txt"));
  arduino_mock::now_ms = 11;
  belowThreshold.process();
  TEST_ASSERT_FALSE(LittleFS.exists("/drd.txt"));
}

void test_multi_reset_detector_detects_consecutive_resets() {
  // Prepare
  resetMocks();
  arduino_mock::reset_reason = ESP_RST_SW;
  MultiResetDetector firstReset(10, 2);
  MultiResetDetector secondReset(10, 2);

  // Perform

  // Asserts
  TEST_ASSERT_FALSE(firstReset.detect());
  TEST_ASSERT_TRUE(secondReset.detect());
  TEST_ASSERT_FALSE(LittleFS.exists("/drd.txt"));
}

void test_multi_reset_detector_ignores_non_software_reset() {
  // Prepare
  resetMocks();
  arduino_mock::reset_reason = ESP_RST_UNKNOWN;
  MultiResetDetector nonSoftwareReset(10, 1);

  // Perform

  // Asserts
  TEST_ASSERT_FALSE(nonSoftwareReset.detect());
  nonSoftwareReset.stop();
  TEST_ASSERT_FALSE(LittleFS.exists("/drd.txt"));
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_multi_reset_detector_removes_marker_after_window);
  RUN_TEST(test_multi_reset_detector_detects_consecutive_resets);
  RUN_TEST(test_multi_reset_detector_ignores_non_software_reset);
  return UNITY_END();
}
