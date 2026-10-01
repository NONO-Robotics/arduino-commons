#pragma once

#include "BoundedPIController.h"
#include "FourWheelBLDCController.h"

/**
 * @brief Four-wheel BLDC controller with front-left encoder feedback.
 *
 * Front-right, rear-left, and rear-right speeds are delegated unchanged to
 * FourWheelBLDCController. Only a copied front-left target receives PI
 * correction before the existing rad/s-to-PWM conversion is applied.
 */
class FourWheelBLDCFeedbackController {
private:
  FourWheelBLDCController controller;
  BoundedPIController frontLeftController;
  float maxW;
  float frontLeftMeasuredSpeed;
  bool hasFrontLeftFeedback;
  unsigned long frontLeftLastUpdateMs;
  bool hasFrontLeftUpdateTime;

public:
  /**
   * @brief Constructs a four-wheel controller with bounded front-left PI feedback.
   *
   * Pin and motor parameters match FourWheelBLDCController. PI parameters are
   * expressed in rad/s and require target and feedback to share one sign
   * convention.
   *
   * @param maxW Maximum angular velocity for the wheels in rad/s.
   * @param minPwm Minimum PWM signal value.
   * @param maxPwm Maximum PWM signal value.
   * @param pinPwmFrontRight PWM pin for front-right motor.
   * @param pinDirFrontRight Direction pin for front-right motor.
   * @param pinBrakeFrontRight Brake pin for front-right motor.
   * @param pinPwmFrontLeft PWM pin for front-left motor.
   * @param pinDirFrontLeft Direction pin for front-left motor.
   * @param pinBrakeFrontLeft Brake pin for front-left motor.
   * @param pinPwmBackRight PWM pin for rear-right motor.
   * @param pinDirBackRight Direction pin for rear-right motor.
   * @param pinBrakeBackRight Brake pin for rear-right motor.
   * @param pinPwmBackLeft PWM pin for rear-left motor.
   * @param pinDirBackLeft Direction pin for rear-left motor.
   * @param pinBrakeBackLeft Brake pin for rear-left motor.
   * @param frontFactor Front motor speed factor.
   * @param backFactor Rear motor speed factor.
   * @param proportionalGain Front-left proportional gain.
   * @param integralGain Front-left integral gain.
   * @param maxCorrection Maximum absolute front-left correction in rad/s.
   * @param maxIntegral Maximum absolute accumulated front-left error.
   */
  FourWheelBLDCFeedbackController(
      float maxW, int minPwm, int maxPwm, int pinPwmFrontRight,
      int pinDirFrontRight, int pinBrakeFrontRight, int pinPwmFrontLeft,
      int pinDirFrontLeft, int pinBrakeFrontLeft, int pinPwmBackRight,
      int pinDirBackRight, int pinBrakeBackRight, int pinPwmBackLeft,
      int pinDirBackLeft, int pinBrakeBackLeft, float frontFactor = 1.0f,
      float backFactor = 1.0f, float proportionalGain = 0.1f,
      float integralGain = 0.01f, float maxCorrection = 0.1f,
      float maxIntegral = 1.0f);

  /**
   * @brief Stores a valid front-left angular speed measurement in rad/s.
   *
   * @param measuredSpeed Front-left measured speed using target sign convention.
   */
  void setFrontLeftFeedback(float measuredSpeed);

  /**
   * @brief Clears front-left feedback and accumulated PI error.
   */
  void invalidateFrontLeftFeedback();

  /**
   * @brief Stops all four motors and clears front-left PI state.
   */
  void stop();

  /**
   * @brief Applies wheel targets with optional bounded front-left PI correction.
   *
   * Without valid feedback, all four targets are delegated unchanged. With
   * feedback, elapsed Arduino time drives PI integration and the corrected
   * front-left target remains within commanded direction and maximum speed.
   *
   * @param fwAngularSpeed Target speed for each wheel in rad/s.
   */
  void applySpeed(const FourWheelAngularSpeed &fwAngularSpeed);
};
