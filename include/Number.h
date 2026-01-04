#ifndef NUMBERS_H
#define NUMBERS_H

/**
 * @brief Check if a value is between min and max (inclusive).
 * @param value Value to check.
 * @param min Minimum value.
 * @param max Maximum value.
 * @return true if min <= value <= max.
 */
bool between(float value, float min, float max) {
  return value >= min && value <= max;
}

#endif // NUMBERS_H