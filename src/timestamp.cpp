#include "timestamp.h"
#include "Logger.h"

void syncClockTimeStamp(const long utcOffsetInSeconds)
{
  // Initialize the time configuration. The second parameter is daylight saving time.
  // If you don't use daylight saving time, leave it at 0.
  configTime(
      utcOffsetInSeconds,
      0,
      "pool.ntp.org",
      "time.nist.gov" // Optional second server for redundancy
  );

  // Wait for NTP time to synchronize.
  // The function 'time(nullptr)' returns a Unix timestamp. A valid value is a large number (> 10^9).
  // If time does not synchronize (e.g. no internet), the loop could hang.
  // Make sure the ESP32 has an internet connection to access the NTP server.
  logger.info("Waiting for NTP time synchronization");
  time_t now = 0;
  while (now < 1000000000)
  { // A valid Unix timestamp is > 10^9 seconds
    now = time(nullptr);
    delay(100);
    Serial.print(".");
  }
  Serial.println("");
  logger.info("NTP time synchronized.");
}