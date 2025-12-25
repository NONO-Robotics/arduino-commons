#pragma once

/**
 * @brief Struct to hold angular velocities for a four-wheeled robot.
 */
struct FourWheelsRobotW {
  float fl; ///< Front Left wheel velocity
  float fr; ///< Front Right wheel velocity
  float bl; ///< Back Left wheel velocity
  float br; ///< Back Right wheel velocity
};