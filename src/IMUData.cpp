#include "IMUData.h"

IMUData::IMUData()
    : orientation_x(0.0), orientation_y(0.0), orientation_z(0.0),
      orientation_w(1.0), // Default to a valid quaternion
      angular_velocity_x(0.0), angular_velocity_y(0.0), angular_velocity_z(0.0),
      linear_acceleration_x(0.0), linear_acceleration_y(0.0),
      linear_acceleration_z(0.0) {}

void IMUData::setOrientation(double w, double x, double y, double z) {
  orientation_w = w;
  orientation_x = x;
  orientation_y = y;
  orientation_z = z;
}

void IMUData::setAngularVelocity(double x, double y, double z) {
  angular_velocity_x = x;
  angular_velocity_y = y;
  angular_velocity_z = z;
}

void IMUData::setLinearAcceleration(double x, double y, double z) {
  linear_acceleration_x = x;
  linear_acceleration_y = y;
  linear_acceleration_z = z;
}
