#include <unity.h>

#include "NativeTestCommon.h"

void test_simple_timer_fires_after_period() {
  // Prepare
  resetMocks();
  SimpleTimer timer(10, onTimer);
  arduino_mock::now_ms = 20;

  // Perform
  timer.update();

  // Asserts
  TEST_ASSERT_EQUAL_INT(1, timerUpdates);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_simple_timer_fires_after_period);
  return UNITY_END();
}
