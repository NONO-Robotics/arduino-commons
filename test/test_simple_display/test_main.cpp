#include <unity.h>

#include "NativeTestCommon.h"

void test_simple_display_render_pipeline() {
  // Prepare
  resetMocks();
  SimpleDisplay display(1, 2);
  u8g2_mock::reset();

  // Perform

  // Asserts
  TEST_ASSERT_EQUAL_PTR(&display, display.clean()->write("native")->render());
  TEST_ASSERT_EQUAL_INT(1, u8g2_mock::clear_calls);
  TEST_ASSERT_EQUAL_INT(1, u8g2_mock::draw_calls);
  TEST_ASSERT_EQUAL_STRING("native", u8g2_mock::last_text.c_str());
  TEST_ASSERT_EQUAL_INT(1, u8g2_mock::send_calls);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_simple_display_render_pipeline);
  return UNITY_END();
}
