#pragma once

/** @brief Logical wheel identities shared by feedback and safety channels. */
enum class WheelPosition : unsigned char {
  FrontRight,
  FrontLeft,
  BackRight,
  BackLeft,
};
