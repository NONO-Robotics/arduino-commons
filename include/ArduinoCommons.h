#pragma once

/**
 * @file ArduinoCommons.h
 * @brief Main include file for the Arduino Commons library.
 *
 * Include this file to access all the features of the library:
 * - Motors (BLDC, DC)
 * - Sensors (GPS, IMU, Magnetic Encoder)
 * - Odometry
 * - Utilities (Logger, Config, Timer, Display)
 */

#include "AS5600Sensor.h"
#include "BLDCMotor.h"
#include "BLDCMotorBuilder.h"
#include "Button.h"
#include "ConfigStorage.h"
#include "DCMotor.h"
#include "DeltaTimeComputer.h"
#include "DifferentialRobotOdometry.h"
#include "EncoderAngularVelocityEstimator.h"
#include "FourWheelAngularSpeed.h"
#include "GPSData.h"
#include "GPSSensor.h"
#include "I2CMultiplexor.h"
#include "IMUData.h"
#include "IMUSensor.h"
#include "MagneticEncoder.h"
#include "MagneticEncoderBuilder.h"
#include "MultiResetDetector.h"
#include "Number.h"
#include "SimpleDisplay.h"
#include "SimpleTimer.h"
#include "StringUtils.h"
#include "WToSignedPWMConverter.h"
#include "timestamp.h"
#include <Arduino.h>