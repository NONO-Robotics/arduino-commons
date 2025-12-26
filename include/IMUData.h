
#pragma once

class IMUData
{
private:
  double orientation_x;
  double orientation_y;
  double orientation_z;
  double orientation_w;

  double angular_velocity_x;
  double angular_velocity_y;
  double angular_velocity_z;

  double linear_acceleration_x;
  double linear_acceleration_y;
  double linear_acceleration_z;

public:
  // Constructor
  IMUData();

  // Setters to modify private data
  void setOrientation(double w, double x, double y, double z);
  void setAngularVelocity(double x, double y, double z);
  void setLinearAcceleration(double x, double y, double z);

  double getOrientationX();
  double getOrientationY();
  double getOrientationZ();
  double getOrientationW();

  double getAngularVelocityX();
  double getAngularVelocityY();
  double getAngularVelocityZ();

  double getLinearAccelerationX();
  double getLinearAccelerationY();
  double getLinearAccelerationZ();
};