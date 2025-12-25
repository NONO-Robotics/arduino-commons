// IMUData.h
#pragma once

class IMUData {
public:
  // Constructor
  IMUData();

  // Setters to modify private data
  void setOrientation(double w, double x, double y, double z);
  void setAngularVelocity(double x, double y, double z);
  void setLinearAcceleration(double x, double y, double z);

  double getOrientationX() const { return orientation_x; }
  double getOrientationY() const { return orientation_y; }
  double getOrientationZ() const { return orientation_z; }
  double getOrientationW() const { return orientation_w; }

private:
  // Data members are now private
  double orientation_x, orientation_y, orientation_z, orientation_w;
  double angular_velocity_x, angular_velocity_y, angular_velocity_z;
  double linear_acceleration_x, linear_acceleration_y, linear_acceleration_z;
};