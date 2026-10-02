#include <unity.h>

#include "NativeTestCommon.h"

void test_to_char_array_copies_string() {
  // Prepare
  resetMocks();

  // Perform
  char* copied = toCharArray("native");

  // Asserts
  TEST_ASSERT_EQUAL_STRING("native", copied);
  delete[] copied;
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_to_char_array_copies_string);
  return UNITY_END();
}
