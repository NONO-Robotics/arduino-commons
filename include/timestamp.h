#pragma once
#include <time.h>

const long AR_UTC_TIME_OFFSET_IN_SECONDS = -3 * 3600;

/**
 * @brief Configures NTP server synchronization.
 *
 * It is CRITICAL for the ESP32 to sync its clock with an NTP server so that
 * odometry message timestamps (and other internal sensor data) match the system
 * time on your PC (where ROS runs).
 *
 * Replace "pool.ntp.org" with a closer NTP server if desired,
 * although pool.ntp.org usually works well.
 *
 * @param utcOffsetInSeconds UTC offset (default: -3 hours for AR/UY/Common).
 */
void syncClockTimeStamp(
    const long utcOffsetInSeconds = AR_UTC_TIME_OFFSET_IN_SECONDS);