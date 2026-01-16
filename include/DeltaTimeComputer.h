#pragma once
#include <Arduino.h>


typedef void (*OnDeltaTimeReachedEvent)();

/**
 * DeltaTimeComputer class for calculating time intervals.
 *
 * This class provides methods to calculate the time difference between
 * consecutive updates, allowing for precise timing in applications.
 */
class DeltaTimeComputer
{
private:
  unsigned int currentTime;
  unsigned int lastTime;
  unsigned int delta;

public:
  /**
   * Constructor for DeltaTimeComputer.
   *
   * @param delta The time period to track in milliseconds.
   */
  DeltaTimeComputer(unsigned int delta = 0);

  /**
   * Setup the DeltaTimeComputer.
   * @return Pointer to the DeltaTimeComputer instance.
   */
  DeltaTimeComputer *setup();

  /**
   * Update the current time and calculate the time difference.
   */
  void update();

  /**
   * Reset the time difference calculation.
   */
  void reset();

  /**
   * Get the time difference in milliseconds.
   *
   * @return The time difference in milliseconds.
   */
  unsigned int deltaInMillis();

  /**
   * Check if a time period has been reached.
   *
   * @return True if the time period has been reached, false otherwise.
   */
  bool hasBeenReached();

  /**
   * Check if a specific time period has been reached.
   *
   * @param deltaParam The specific time period to check against in milliseconds.
   * @return True if the time period has been reached, false otherwise.
   */
  bool hasBeenReached(unsigned int deltaParam);

  void update(OnDeltaTimeReachedEvent event);

  /**
   * Get the configured delta time.
   *
   * @return The configured delta time in milliseconds.
   */
  unsigned int getDelta();
};
