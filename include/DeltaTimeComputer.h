#pragma once
#include <Arduino.h>


typedef void (*OnDeltaTimeReachedEvent)();

/**
 * @brief Tracks elapsed Arduino time between updates and configured intervals.
 *
 * This class provides methods to calculate the time difference between
 * consecutive updates, allowing for precise timing in applications.
 * 
 * Usage Context: Crucial for kinematics and odometry (e.g. integrating
 * velocity to position over time). Provides accurate `dt` measurement
 * between loop iterations to ensure mathematics mirror physical reality.
 */
class DeltaTimeComputer
{
private:
  unsigned int currentTime;
  unsigned int lastTime;
  unsigned int delta;

public:
  /**
   * @brief Creates an interval tracker with an optional target period.
   * @param delta Target period in milliseconds; defaults to zero.
   */
  DeltaTimeComputer(unsigned int delta = 0);

  /**
   * @brief Initializes time tracking from the current Arduino clock value.
   * @return Pointer to this initialized DeltaTimeComputer instance.
   */
  DeltaTimeComputer *setup();

  /**
   * @brief Updates the elapsed time using the current Arduino clock value.
   */
  void update();

  /**
   * @brief Resets elapsed-time tracking to the current Arduino clock value.
   */
  void reset();

  /**
   * @brief Returns elapsed time since the preceding update or reset.
   * @return Elapsed time in milliseconds.
   */
  unsigned int deltaInMillis();

  /**
   * @brief Tests whether elapsed time reaches the configured target period.
   * @return True when elapsed milliseconds are at least the configured period.
   */
  bool hasBeenReached();

  /**
   * @brief Tests whether elapsed time reaches a caller-provided period.
   * @param deltaParam Period to compare against in milliseconds.
   * @return True when elapsed milliseconds are at least deltaParam.
   */
  bool hasBeenReached(unsigned int deltaParam);

  /**
   * @brief Updates time and invokes a callback when the configured period is reached.
   * @param event Callback invoked when elapsed milliseconds reach the configured period.
   */
  void update(OnDeltaTimeReachedEvent event);

  /**
   * @brief Returns the configured target period.
   * @return Target period in milliseconds.
   */
  unsigned int getDelta();
};
