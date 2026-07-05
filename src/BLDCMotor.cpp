#include "BLDCMotor.h"

// Constructor
BLDCMotor::BLDCMotor(int pwmPin, int dirPin, int brakePin, int resolutionInBits,
                     int channel, float frequency, bool invertDirection)
    : pwmPin(pwmPin), dirPin(dirPin), brakePin(brakePin), channel(channel),
      frequency(frequency), resolutionInBits(resolutionInBits),
      invertDirection(invertDirection) {
  currentSpeed = 0;
}

int BLDCMotor::getResolutionInBits() { return resolutionInBits; }

// Pin Initialization
BLDCMotor *BLDCMotor::setup() {
  pinMode(pwmPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(brakePin, OUTPUT);

  // Configure PWM using ESP32 LEDC (Core v2.x compatible)
  ledcSetup(channel, frequency, resolutionInBits);
  ledcAttachPin(pwmPin, channel);

  // Calculate maximum PWM value based on resolution (2^bits - 1)
  pwmMax = (1 << resolutionInBits) - 1;

  releaseBrake();
  return this;
}

int getSign(float value) { return (value >= 0) ? 1 : -1; }

// Set Speed and Direction
BLDCMotor *BLDCMotor::setPwmSpeed(int speed) {
  // Limit the value to the allowed range (e.g., -4095 to 4095)
  speed = constrain(speed, -pwmMax, pwmMax);

  // 1. Zero Speed Case
  if (speed == 0) {
    ledcWrite(channel, 0);
    this->currentSpeed = 0;
    return this; // Do not activate physical brake here, only inertia
  }

  // 2. Direction Control
  // Note: Removed automatic stop() logic to avoid blocking.
  // The upper Ramp controller will handle passing through 0 smoothly.
  if (speed > 0) {
    releaseBrake();
    digitalWrite(dirPin, invertDirection ? DIR_REVERSE : DIR_FORWARD);
  } else {
    digitalWrite(dirPin, invertDirection ? DIR_FORWARD : DIR_REVERSE);
    releaseBrake();
  }

  // 3. PWM Write (ONLY ledcWrite)
  // Use abs() because PWM duty cycle is always positive
  ledcWrite(channel, abs(speed));

  this->currentSpeed = speed;
  return this;
}

BLDCMotor *BLDCMotor::brake() {
  digitalWrite(brakePin, LOW);  // Activate physical brake (Active-Low)
  ledcWrite(channel, 0);        // Ensure PWM is 0
  this->currentSpeed = 0;
  return this;
}

BLDCMotor *BLDCMotor::releaseBrake() {
  digitalWrite(brakePin, HIGH); // Release brake (Active-Low)
  // Do not write PWM here, wait for the next setPwmSpeed call
  return this;
}

BLDCMotor *BLDCMotor::stop() {
  // Stop simply activates the brake without waiting time
  this->brake();
  return this;
};