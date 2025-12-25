#pragma once

#include <TinyGPSPlus.h>

class GPSData {
private:
  TinyGPSPlus data;

public:
  void encode(char c);
  bool isLocationUpdated() const;

  bool isLocationValid() const;

  double getLatitude();

  double getLongitude();

  double getAltitude();

  int getSatellites();

  // Devuelve el HDOP (Horizontal Dilution of Precision) para covarianza
  double getHDOP();

  int getTimeMinute();

  int getTimeSecond();

  int getTimeHour();
};
