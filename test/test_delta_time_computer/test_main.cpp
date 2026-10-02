#include <unity.h>

#include "NativeTestCommon.h"

void test_delta_time_computer_first_update_does_not_fire() {
  // Prepare
  resetMocks();
  DeltaTimeComputer delta(10);
  delta.setup();
  arduino_mock::now_ms = 11;

  // Perform
  delta.update(onTimer);

  // Asserts
  TEST_ASSERT_EQUAL_INT(0, timerUpdates);
}

void test_delta_time_computer_fires_on_second_update() {
  // Prepare
  resetMocks();
  DeltaTimeComputer delta(10);
  delta.setup();
  arduino_mock::now_ms = 11;
  delta.update(onTimer);

  // Perform
  delta.update(onTimer);

  // Asserts
  TEST_ASSERT_EQUAL_INT(1, timerUpdates);
  TEST_ASSERT_EQUAL_UINT(10, delta.getDelta());
}

void test_delta_time_computer_ignores_null_before_interval() {
  // Prepare
  resetMocks();
  DeltaTimeComputer delta(10);
  delta.setup();
  arduino_mock::now_ms = 11;
  delta.update(onTimer);
  delta.update(onTimer);

  // Perform
  delta.update(nullptr);

  // Asserts
  TEST_ASSERT_EQUAL_INT(1, timerUpdates);
}

void test_delta_time_computer_null_callback_when_interval_reached() {
  // Prepare
  resetMocks();
  DeltaTimeComputer delta(10);
  delta.setup();
  arduino_mock::now_ms = 11;
  delta.update(onTimer);
  delta.update(onTimer);
  delta.update(nullptr);
  arduino_mock::now_ms = 31;

  // Perform
  delta.update(nullptr);
  delta.update(nullptr);

  // Asserts
  TEST_ASSERT_EQUAL_INT(1, timerUpdates);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_delta_time_computer_first_update_does_not_fire);
  RUN_TEST(test_delta_time_computer_fires_on_second_update);
  RUN_TEST(test_delta_time_computer_ignores_null_before_interval);
  RUN_TEST(test_delta_time_computer_null_callback_when_interval_reached);
  return UNITY_END();
}
