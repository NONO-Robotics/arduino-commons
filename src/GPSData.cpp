#include "GPSData.h"

void GPSData::encode(char c) { data.encode(c); }

bool GPSData::isLocationUpdated() const { return data.location.isUpdated(); }

bool GPSData::isLocationValid() const { return data.location.isValid(); }

double GPSData::getLatitude() { return data.location.lat(); }

double GPSData::getLongitude() { return data.location.lng(); }

double GPSData::getAltitude() { return data.altitude.meters(); }

int GPSData::getSatellites() { return data.satellites.value(); }

// Returns the HDOP (Horizontal Dilution of Precision) for covariance
double GPSData::getHDOP() {
  return data.hdop.isValid() ? data.hdop.value() : 99.0;
}

int GPSData::getTimeMinute() {
  return data.time.isValid() ? data.time.minute() : 0;
}

int GPSData::getTimeSecond() {
  return data.time.isValid() ? data.time.second() : 0;
}

int GPSData::getTimeHour() {
  return data.time.isValid() ? data.time.hour() : 0;
}