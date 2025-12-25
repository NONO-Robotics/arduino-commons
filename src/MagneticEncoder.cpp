#include "MagneticEncoder.h"

MagneticEncoder::MagneticEncoder(OnUpdateWEvent cb, short int channel,
                                 unsigned long sampleIntervalMs, double alpha,
                                 bool applyFilter, float deadZone, int address,
                                 TwoWire *i2cPort)
    : sensor(sensor), // Initialize sensor pointer
      sampleIntervalMs(sampleIntervalMs),
      onUpdateEvent(cb), // Assign new callback function
      previousStep(0),   // Initialize previous value to 0
      previousUpdateTimeMs(0) {
  this->channel = channel;
  this->applyFilter = applyFilter;
  sensor = new AS5600Sensor(i2cPort, address);
  wEstimator = new EncoderAngularVelocityEstimator(alpha, deadZone);
  currentW = 0;
  currentStep = 0;
}

// Initialize the wheel sensor
bool MagneticEncoder::begin() {
  if (sensor->begin()) {               // Try to initialize AS5600 sensor
    previousStep = sensor->getValue(); // Get initial value
    previousUpdateTimeMs = millis();   // Register start time
    return true;
  } else {
    logger.error("Encoder '" + String(channel) + "': Initialization fail.");
    return false;
  }
}

// Updates the wheel sensor state and calculates angular velocity
void MagneticEncoder::update() {
  unsigned long currentTimeMs = millis(); // Get current time once

  // Check if desired sampling interval has passed
  if ((currentTimeMs - previousUpdateTimeMs) >= sampleIntervalMs) {
    sensor->update(); // Perform a new reading from AS5600 sensor

    if (sensor->isSuccessful()) {
      currentStep = sensor->getValue();

      // Calculate raw angle difference, handling "wrap-around" (0-4095 range)
      // The AS5600 has a 12-bit resolution (0-4095).
      int diffRaw = (int)currentStep - (int)previousStep;

      // Handle wrap-around cases:
      // If the difference is very large positive, it means we wrapped from 4095
      // to 0 (forward)
      if (diffRaw > 2048) {
        diffRaw -= 4096;
      }
      // If the difference is very large negative, it means we wrapped from 0 to
      // 4095 (reverse)
      else if (diffRaw < -2048) {
        diffRaw += 4096;
      }

      // Calculate time difference in milliseconds
      unsigned long deltaTimeMs = currentTimeMs - previousUpdateTimeMs;

      currentW =
          wEstimator->getWInRadBySec(diffRaw, deltaTimeMs, this->applyFilter);

      onUpdateEvent(channel, currentStep, currentW);

      // Update previous values for next iteration,
      // regardless of whether threshold was exceeded. This ensures that
      // angular velocity is always calculated from the last successful reading.
      previousStep = currentStep;
      previousUpdateTimeMs = currentTimeMs;
    } else {
      logger.error("Encoder '" + String(channel) + "': Cant read a value.");
    }
  }
}

short int MagneticEncoder::getChannel() { return channel; }

float MagneticEncoder::getW() { return currentW; }

uint16_t MagneticEncoder::getStep() { return currentStep; }
