# Arduino Commons

<div align="center">
  <img src="https://img.shields.io/badge/PlatformIO-Compatible-orange" alt="PlatformIO"/>
  <img src="https://img.shields.io/badge/Arduino-Compatible-blue" alt="Arduino"/>
  <img src="https://img.shields.io/badge/License-MIT-green" alt="License"/>
</div>

<div align="center">
  <p><strong>A robust collection of common utilities, drivers, and algorithms for robotics development on Arduino and ESP32.</strong></p>
</div>

---

## 📦 Ecosystem

This library is part of the **4w-ros-robot** project family.

| Project | Role | Description |
| :--- | :--- | :--- |
| **[4w-ros-robot](https://github.com/adrianmarino/4w-ros-robot)** | **Main** | The central repository for the autonomous 4-wheeled robot project. |
| **[4w-robot-ros-movement](https://github.com/adrianmarino/4w-robot-ros-movement)** | Firmware | Movement controller firmware handling PID and kinematics. |
| **[4w-robot-ros-w-publisher](https://github.com/adrianmarino/4w-robot-ros-w-publisher)** | Firmware | Publishes wheel angular velocities using magnetic encoders. |
| **[4w-robot-ros-imu-gps](https://github.com/adrianmarino/4w-ros-robot-imu-gps)** | Firmware | Sensor fusion node for IMU (BNO08x) and GPS data. |
| **[arduino-ros](https://github.com/adrianmarino/arduino-ros)** | Library | Common ROS generic implementations for micro-ROS integration. |

## 🚀 Installation

### PlatformIO (Recommended)
Add the following to your `platformio.ini`:

```ini
[env:my_env]
lib_deps =
    adrianmarino/arduino-commons
    # Add other dependencies like Adafruit BNO08x, U8g2 if using specific modules
```

### Arduino IDE
1. Clone this repository into your `Arduino/libraries` folder.
2. Restart the IDE.

---

## 📖 API & Usage Examples

### 🚗 Motors

#### `BLDCMotor` & `BLDCMotorBuilder`
Control Brushless DC motors with PWM, direction, and brake support.

```cpp
#include <BLDCMotorBuilder.h>

BLDCMotor* motor;

void setup() {
    // Fluent builder pattern for easy configuration
    motor = BLDCMotorBuilder(5, 18, 19) // PWM, DIR, BRAKE pins
                .setChannel(0)
                .setFrequency(20000)
                .setResolutionInBits(11)
                .build();

    motor->setup();
    motor->setPwmSpeed(500); // Set speed
}

void loop() {
    // Motor logic
}
```

#### `DCMotor`
Simple driver for DC Generic motors using H-Bridge drivers.

```cpp
#include <DCMotor.h>

// A-Pin, B-Pin, PWM-Pin
DCMotor motor(12, 13, 14);

void setup() {
    motor.setup();
    motor.move(200); // Forward speed 0-255
}
```

---

### 📡 Sensors

#### `GPSSensor`
Wrapper for serial GPS modules, utilizing callbacks for non-blocking updates.

```cpp
#include <GPSSensor.h>

void onGpsUpdate(GPSData* data) {
    // Process new GPS data
    Serial.print("Lat: "); Serial.println(data->lat);
}

GPSSensor* gps;

void setup() {
    Serial2.begin(9600);
    
    gps = GPSSensor::GPSSensorBuilder(&Serial2)
            .setPins(16, 17) // RX, TX
            .setOnUpdateEvent(onGpsUpdate)
            .build();
}

void loop() {
    // Internal loop handling is done typically inside standard loop or RTOS task
}
```

#### `IMUSensor`
Driver for BNO08x IMU over I2C, providing orientation and acceleration data.

```cpp
#include <IMUSensor.h>

void onImuUpdate(IMUData* data) {
    Serial.print("Yaw: "); Serial.println(data->yaw);
}

IMUSensor imu(onImuUpdate);

void setup() {
    imu.init();
    imu.begin(); // Setup default intervals
}

void loop() {
    imu.update(); // Poll sensor
}
```

#### `MagneticEncoder` (AS5600)
Reads angular position and estimates angular velocity ($w$) from AS5600 magnetic sensors.

```cpp
#include <MagneticEncoderBuilder.h>

void onVelocityUpdate(short int channel, int step, float w) {
    Serial.printf("Ch: %d, Speed: %.2f rad/s\n", channel, w);
}

MagneticEncoder* encoder;

void setup() {
    Wire.begin();
    
    encoder = MagneticEncoderBuilder()
                .setChannel(0)
                .setI2CPort(&Wire)
                .setCallback(onVelocityUpdate)
                .build();
                
    encoder->begin();
}

void loop() {
    encoder->update(); // Calculate velocity and trigger callback if needed
}
```

---

### 🛠 Utilities

#### `SimpleTimer`
Execute tasks periodically without blocking `loop()`.

```cpp
#include <SimpleTimer.h>

void blinkParam(int pin) {
    digitalWrite(pin, !digitalRead(pin));
}

// execute callback every 1000ms
SimpleTimer timer(1000, []() {
    Serial.println("Tick!");
});

void setup() {
    // ...
}

void loop() {
    timer.update(); // Checks time and runs task if ready
}
```

#### `SimpleDisplay`
Wrapper for SSD1306/SH1106 OLED displays using `U8g2`.

```cpp
#include <SimpleDisplay.h>

SimpleDisplay display(SDA, SCL);

void setup() {
    display.write("Hello World")
           ->render();
}
```

#### `DeltaTimeComputer`
Calculates high-precision `dt` for valid integration in control loops.

```cpp
#include <DeltaTimeComputer.h>

DeltaTimeComputer dtComputer;

void loop() {
    dtComputer.update();
    float dt = dtComputer.deltaInMillis() / 1000.0f;
    
    // Use dt for PID or Odometry
}
```

#### `I2CMultiplexor`
Switch channels on TCA9548A multiplexers to use multiple devices with the same I2C address.

```cpp
#include <I2CMultiplexor.h>

I2CMultiplexor mux(0x70);

void setup() {
    mux.selectChannel(2);
    // Now talk to device on channel 2
}
```

## ⚖️ License

This project is licensed under the MIT License - see the LICENSE file for details.
