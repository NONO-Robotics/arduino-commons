#include <unity.h>

#include "NativeTestCommon.h"

void test_sync_clock_time_stamp_waits_for_ntp() {
  // Prepare
  resetMocks();
  Serial.clear();

  // Perform
  syncClockTimeStamp(0);

  // Asserts
  TEST_ASSERT_EQUAL_STRING(".\n", Serial.output().c_str());
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_sync_clock_time_stamp_waits_for_ntp);
  return UNITY_END();
}
