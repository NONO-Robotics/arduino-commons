#pragma once

#include <FourWheelAngularSpeed.h>
#include <FourWheelBLDCController.h>
#include <WheelSpeeds.h>
#include <WheelPosition.h>
#include <WheelSpeedFeedbackController.h>

/**
 * @brief Drives four logical Outdoor wheels with independent optional PI feedback.
 *
 * Pin arguments must match FourWheelBLDCController logical wheel order. Disabled
 * channels pass targets through unchanged. Each enabled channel owns its own
 * command/wheel-speed recovery state; any unsafe active channel brakes all motors.
 */
class OutdoorFourWheelFeedbackController {
 private:
  FourWheelBLDCController controller_;
  WheelSpeedFeedbackController frontRight_;
  WheelSpeedFeedbackController frontLeft_;
  WheelSpeedFeedbackController backRight_;
  WheelSpeedFeedbackController backLeft_;
  float minW_;
  float maxW_;
  const float frontRightSpeedScale_;
  const float frontLeftSpeedScale_;
  const float backRightSpeedScale_;
  const float backLeftSpeedScale_;
  const unsigned long feedbackTimeoutMs_;

  WheelSpeedFeedbackController &channel(WheelPosition wheel);
  static float targetFor(const FourWheelAngularSpeed &speed, WheelPosition wheel);
  static void setTarget(FourWheelAngularSpeed &speed, WheelPosition wheel,
                        float target);

 public:
  /**
   * @brief Creates four-wheel actuator and fixed feedback channels.
   *
   * @param minW Smallest corrected nonzero target producing usable torque.
   * @param feedbackTimeoutMs Maximum valid diagnostic age.
   * @param frontRightMotor Front-right motor controller settings.
   * @param frontLeftMotor Front-left motor controller settings.
   * @param backRightMotor Back-right motor controller settings.
   * @param backLeftMotor Back-left motor controller settings.
   * @param frontRight Front-right feedback configuration.
   * @param frontLeft Front-left feedback configuration.
   * @param backRight Back-right feedback configuration.
   * @param backLeft Back-left feedback configuration.
  */
  OutdoorFourWheelFeedbackController(
      float minW, unsigned long feedbackTimeoutMs,
      const BLDCMotorSettings &frontRightMotor,
      const BLDCMotorSettings &frontLeftMotor,
      const BLDCMotorSettings &backRightMotor,
      const BLDCMotorSettings &backLeftMotor,
      const WheelFeedbackSettings &frontRight,
      const WheelFeedbackSettings &frontLeft,
      const WheelFeedbackSettings &backRight,
      const WheelFeedbackSettings &backLeft);

  /** @brief Propagates one received command to every feedback channel. */
  void receiveFreshCommand();

  /**
   * @brief Routes validated wheel speeds to all feedback channels.
   *
   * Converts each received wheel speed using its configured scale and assigns
   * one timestamp to the complete wheel-speed update.
   *
   * @param wheelSpeeds Validated wheel angular speeds received from ROS.
   */
  void updateFeedback(const WheelSpeeds &wheelSpeeds);

  /** @brief Latches a fault in one wheel feedback channel. */
  void faultFeedback(WheelPosition wheel);

  /** @brief Latches feedback faults in every wheel channel. */
  void faultAllFeedback();

  /** @brief Brakes every motor and resets every PI integrator. */
  void stop();

  /**
   * @brief Applies logical targets when every active enabled channel is safe.
   *
   * Only a corrected, nonzero target is floored to minW. Zero targets reset
   * their channel integrator; disabled channels remain exact passthrough. An
   * unsafe enabled active channel brakes all motors and returns false.
   *
   * @param fwAngularSpeed Requested logical wheel targets.
   * @return True when targets were applied; false when safety braked motors.
  */
  bool applySpeed(const FourWheelAngularSpeed &fwAngularSpeed);
};
