#pragma once

#include <TinyGPSPlus.h>

/**
 * @brief Data structure holding parsed GPS information.
 *
 * Wraps TinyGPSPlus to provide easy access to location, time, and quality
 * metrics.
 */
class GPSData {
private:
  TinyGPSPlus data;

public:
  /**
   * @brief Process a character from the GPS stream.
   * @param c Character received from GPS module.
   */
  void encode(char c);

  /**
   * @brief Check if location has been updated.
   * @return true if location is valid and updated.
   */
  bool isLocationUpdated() const;

  /**
   * @brief Check if location is valid.
   * @return true if valid.
   */
  bool isLocationValid() const;

  /**
   * @brief Get Latitude.
   * @return Latitude in degrees.
   */
  double getLatitude();

  /**
   * @brief Get Longitude.
   * @return Longitude in degrees.
   */
  double getLongitude();

  /**
   * @brief Get Altitude.
   * @return Altitude in meters.
   */
  double getAltitude();

  /**
   * @brief Get number of visible satellites.
   * @return Number of satellites.
   */
  int getSatellites();

  /**
   * @brief Get Horizontal Dilution of Precision (HDOP).
   * @return HDOP value (lower is better).
   */
  double getHDOP();

  /**
   * @brief Get current Minute.
   * @return Minute (0-59).
   */
  int getTimeMinute();

  /**
   * @brief Get current Second.
   * @return Second (0-59).
   */
  int getTimeSecond();

  /**
   * @brief Get current Hour.
   * @return Hour (0-23).
   */
  int getTimeHour();
};
