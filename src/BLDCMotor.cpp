#include "BLDCMotor.h"
#include <esp_arduino_version.h>

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

#if defined(ESP_ARDUINO_VERSION) && ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 0, 0)
  // Configure PWM using ESP32 LEDC (Core v3.x compatible)
  ledcAttach(pwmPin, frequency, resolutionInBits);
#else
  // Configure PWM using ESP32 LEDC (Core v2.x compatible)
  ledcSetup(channel, frequency, resolutionInBits);
  ledcAttachPin(pwmPin, channel);
#endif

  // Calculate maximum PWM value based on resolution (2^bits - 1)
  pwmMax = (1 << resolutionInBits) - 1;

  releaseBrake();
  return this;
}

// Set Speed and Direction
BLDCMotor *BLDCMotor::setPwmSpeed(int speed) {
  // Limit the value to the allowed range (e.g., -4095 to 4095)
  speed = constrain(speed, -pwmMax, pwmMax);

  // 1. Zero Speed Case
  if (speed == 0) {
#if defined(ESP_ARDUINO_VERSION) && ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 0, 0)
    ledcWrite(pwmPin, 0);
#else
    ledcWrite(channel, 0);
#endif
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
#if defined(ESP_ARDUINO_VERSION) && ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 0, 0)
  ledcWrite(pwmPin, abs(speed));
#else
  ledcWrite(channel, abs(speed));
#endif

  this->currentSpeed = speed;
  return this;
}

BLDCMotor *BLDCMotor::brake() {
  digitalWrite(brakePin, HIGH); // Activate physical brake
#if defined(ESP_ARDUINO_VERSION) && ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 0, 0)
  ledcWrite(pwmPin, 0);        // Ensure PWM is 0
#else
  ledcWrite(channel, 0);        // Ensure PWM is 0
#endif
  this->currentSpeed = 0;
  return this;
}

BLDCMotor *BLDCMotor::releaseBrake() {
  digitalWrite(brakePin, LOW); // Release brake
  // Do not write PWM here, wait for the next setPwmSpeed call
  return this;
}

BLDCMotor *BLDCMotor::stop() {
  // Stop simply activates the brake without waiting time
  this->brake();
  return this;
}