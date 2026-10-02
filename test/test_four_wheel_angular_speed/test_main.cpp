#include <cassert>
#include <unity.h>

#include "FourWheelAngularSpeed.h"
#include "NativeTestCommon.h"

void test_wheel_speed_averages() {
  // Prepare
  FourWheelAngularSpeed speeds;
  speeds.updateFrom(-2.0F, 6.0F, 4.0F, -8.0F);

  // Perform

  // Asserts
  TEST_ASSERT_EQUAL_FLOAT(1.0F, speeds.getAverageLeftWInRad());
  TEST_ASSERT_EQUAL_FLOAT(-1.0F, speeds.getAverageRightWInRad());
}

void test_four_wheel_less_than_above_threshold_false() {
  // Prepare
  resetMocks();
  FourWheelAngularSpeed speeds;
  speeds.updateFrom(-2.0F, 6.0F, 4.0F, -8.0F);

  // Perform

  // Asserts
  TEST_ASSERT_FALSE(speeds.lessThan(4.0F));
}

void test_four_wheel_less_than_below_threshold_true() {
  // Prepare
  resetMocks();
  FourWheelAngularSpeed speeds;
  speeds.updateFrom(1.0F, 1.0F, 1.0F, 1.0F);

  // Perform

  // Asserts
  TEST_ASSERT_TRUE(speeds.lessThan(2.0F));
}

void test_four_wheel_less_than_false_when_front_left_above() {
  // Prepare
  resetMocks();
  FourWheelAngularSpeed speeds;
  speeds.updateFrom(1.0F, 1.0F, 1.0F, 1.0F);
  speeds.setFlWInRad(2.0F);

  // Perform

  // Asserts
  TEST_ASSERT_FALSE(speeds.lessThan(2.0F));
}

void test_four_wheel_less_than_false_when_front_right_above() {
  // Prepare
  resetMocks();
  FourWheelAngularSpeed speeds;
  speeds.updateFrom(1.0F, 1.0F, 1.0F, 1.0F);
  speeds.setFrWInRad(2.0F);

  // Perform

  // Asserts
  TEST_ASSERT_FALSE(speeds.lessThan(2.0F));
}

void test_four_wheel_less_than_false_when_back_left_above() {
  // Prepare
  resetMocks();
  FourWheelAngularSpeed speeds;
  speeds.updateFrom(1.0F, 1.0F, 1.0F, 1.0F);
  speeds.setBlWInRad(2.0F);

  // Perform

  // Asserts
  TEST_ASSERT_FALSE(speeds.lessThan(2.0F));
}

void test_four_wheel_less_than_false_when_back_right_above() {
  // Prepare
  resetMocks();
  FourWheelAngularSpeed speeds;
  speeds.updateFrom(1.0F, 1.0F, 1.0F, 1.0F);
  speeds.setBrWInRad(2.0F);

  // Perform

  // Asserts
  TEST_ASSERT_FALSE(speeds.lessThan(2.0F));
}


int main() {
  UNITY_BEGIN();
  RUN_TEST(test_four_wheel_less_than_above_threshold_false);
  RUN_TEST(test_four_wheel_less_than_below_threshold_true);
  RUN_TEST(test_four_wheel_less_than_false_when_front_left_above);
  RUN_TEST(test_four_wheel_less_than_false_when_front_right_above);
  RUN_TEST(test_four_wheel_less_than_false_when_back_left_above);
  RUN_TEST(test_four_wheel_less_than_false_when_back_right_above);
  RUN_TEST(test_wheel_speed_averages);
  return UNITY_END();
}
