#include <unity.h>

#include "NativeTestCommon.h"

void test_button_press_fires_once_when_debounced() {
  // Prepare
  resetMocks();
  Button button(7);
  button.init();
  arduino_mock::digital_values[7] = LOW;
  arduino_mock::now_ms += 51;

  // Perform

  // Asserts
  TEST_ASSERT_EQUAL_INT(INPUT_PULLUP, arduino_mock::pin_modes[7]);
  TEST_ASSERT_TRUE(button.pressed());
  TEST_ASSERT_FALSE(button.pressed());
}

void test_button_release_after_debounce_returns_false() {
  // Prepare
  resetMocks();
  Button button(7);
  button.init();
  arduino_mock::digital_values[7] = LOW;
  arduino_mock::now_ms += 51;
  button.pressed();

  // Perform
  arduino_mock::digital_values[7] = HIGH;
  arduino_mock::now_ms += 51;

  // Asserts
  TEST_ASSERT_FALSE(button.pressed());
}

void test_button_ignores_change_inside_debounce_window() {
  // Prepare
  resetMocks();
  Button button(7);
  button.init();
  arduino_mock::digital_values[7] = LOW;
  arduino_mock::now_ms += 51;
  button.pressed();
  arduino_mock::digital_values[7] = HIGH;
  arduino_mock::now_ms += 51;
  button.pressed();

  // Perform
  arduino_mock::digital_values[7] = LOW;
  arduino_mock::now_ms += 10;

  // Asserts
  TEST_ASSERT_FALSE(button.pressed());
}

void test_button_press_accepted_after_debounce_window() {
  // Prepare
  resetMocks();
  Button button(7);
  button.init();
  arduino_mock::digital_values[7] = LOW;
  arduino_mock::now_ms += 51;
  button.pressed();
  arduino_mock::digital_values[7] = HIGH;
  arduino_mock::now_ms += 51;
  button.pressed();
  arduino_mock::digital_values[7] = LOW;
  arduino_mock::now_ms += 10;
  button.pressed();

  // Perform
  arduino_mock::now_ms += 51;

  // Asserts
  TEST_ASSERT_TRUE(button.pressed());
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_button_press_fires_once_when_debounced);
  RUN_TEST(test_button_release_after_debounce_returns_false);
  RUN_TEST(test_button_ignores_change_inside_debounce_window);
  RUN_TEST(test_button_press_accepted_after_debounce_window);
  return UNITY_END();
}
