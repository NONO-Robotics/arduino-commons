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


## 🌐 Ecosystem

This project is part of the **4w-ros-robot** family:

> 💡 **Naming Convention Tip:**
> * All subrepositories that **do not** have the prefix `outdoor` are used for the **Indoor Robot** version (except for shared utility libraries like `arduino-commons` and `arduino-ros`).
> * Subrepositories specifically belonging to the **Outdoor Robot** are prefixed with `outdoor` (with the exception of `4w-robot-cutting-control` which is specific to the grass-cutting system).

* [4w-ros-robot](https://github.com/adrianmarino/4w-ros-robot)
  * **Sensor data publisher firmware**
      * [4w-robot-ros-lidar](https://github.com/adrianmarino/4w-robot-ros-lidar): LIDAR sensor publisher firmware.
  * **Libraries**
    * [arduino-ros](https://github.com/adrianmarino/arduino-ros): ROS common library.
    * [arduino-commons](https://github.com/adrianmarino/arduino-commons): Arduino common library.
  * **Design**
    * [4w-robot-ros-kicad](https://github.com/adrianmarino/4w-robot-ros-kicad) PCB's Design.
    * [Solidworks Model](https://drive.google.com/drive/folders/1mQg-BSRZyyYhnBoig6Qm0Zf43U8bTAA7?usp=sharing): 3D model design.
  * **Indoor**
    * **Navigation**
      * [4w-robot-ros-ws](https://github.com/adrianmarino/4w-robot-ros-ws): Autonomous/manual navigation control project.
      * [4w-robot-ros-movement](https://github.com/adrianmarino/4w-robot-ros-movement.git): Movement controller firmware.
    * **Sensor data publisher firmware**
      * [4w-robot-ros-w-publisher](https://github.com/adrianmarino/4w-robot-ros-w-publisher): Wheels angular velocity sensors publisher firmware.
      * [4w-robot-ros-imu-gps](https://github.com/adrianmarino/4w-ros-robot-imu-gps): IMU, GPS sensors publisher firmware.
  * **Outdoor**
    * [mowbot-connect](https://github.com/adrianmarino/mowbot-connect): Real-time Web Control HUD and management application.
    * [4w-robot-cutting-control](https://github.com/adrianmarino/4w-robot-cutting-control): Automatic cutting motor controller.
    * **Navigation**
      * [4w-outdoor-robot-ros-ws](https://github.com/adrianmarino/4w-outdoor-robot-ros-ws): Autonomous/manual navigation control project.
      * [4w-outdoor-robot-ros-movement](https://github.com/adrianmarino/4w-outdoor-robot-ros-movement.git): Outdoor Movement controller firmware.
    * **Sensor data publisher firmware**
      * [4w-outdoor-robot-ros-w-publisher](https://github.com/adrianmarino/4w-outdoor-robot-ros-w-publisher): Outdoor wheels angular velocity sensors publisher firmware.
      * [4w-outdoor-robot-ros-imu-gps](https://github.com/adrianmarino/4w-outdoor-ros-robot-imu-gps): IMU, GPS sensors publisher firmware.


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

## 📖 Documentation

See [Documentation Site](https://nono-robotics.github.io/arduino-commons/)

---

## 📖 API & Usage Examples

### 🚗 Motors

#### `FourWheelBLDCController` & `BLDCMotorController`
High-level controllers for 4-wheel bases and single BLDC motors, handling angular velocities and PWM translation.
* **Usage Context**: `FourWheelBLDCController` acts as the central controller in outdoor/4x4 independent drive robots (e.g. `4w-outdoor-robot-ros-movement`). It receives target wheel speeds (calculated from ROS Twist messages via kinematic equations) and applies them simultaneously to all four wheels.

```cpp
#include <FourWheelBLDCController.h>

FourWheelBLDCController* baseController;

void setup() {
    baseController = new FourWheelBLDCController(
        10.0, 50, 255,   // maxW, minPwm, maxPwm
        1, 2, 3,         // FR pins
        4, 5, 6,         // FL pins
        7, 8, 9,         // BR pins
        10, 11, 12       // BL pins
    );
    // Control base with rad/s
    FourWheelAngularSpeed speeds;
    speeds.updateFrom(2.5, 2.5, 2.5, 2.5);
    baseController->applySpeed(speeds);
}
```

#### `BLDCMotor` & `BLDCMotorBuilder`
Control Brushless DC motors with PWM, direction, and brake support.

```cpp
#include <BLDCMotorBuilder.h>

BLDCMotor* motor;

void setup() {
    // Fluent builder pattern for easy configuration
    motor = (new BLDCMotorBuilder(5, 18, 19)) // PWM, DIR, BRAKE pins
                ->setChannel(0)
                ->setFrequency(20000)
                ->setResolutionInBits(11)
                ->build();

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

#### `AS5600Sensor`, `MagneticEncoderUpdateService` & `EncoderAngularVelocityEstimator`
Direct I2C interface for AS5600, service builder for multiplexed encoders, and angular velocity estimation.
* **Usage Context**: 
  * `MagneticEncoderUpdateService`: Since AS5600 sensors have a fixed physical I2C address, it's impossible to connect four of them directly to the same bus. This service works with an I2C Multiplexor (e.g., TCA9548A) to rapidly poll all 4 wheels in the wheel-publisher nodes.
  * `EncoderAngularVelocityEstimator`: Essential for processing noisy raw data from magnetic encoders. It implements an Exponentially Weighted Moving Average (EWMA) filter to smooth out spikes and provide stable velocity estimates (rad/s) for reliable odometry calculation.

```cpp
#include <MagneticEncoderUpdateServiceBuilder.h>
#include <EncoderAngularVelocityEstimator.h>

MagneticEncoderUpdateService* encoderService;

void onEncoderUpdate(short int channel, int step, float w) {
    Serial.printf("Ch %d speed: %f\n", channel, w);
}

void setup() {
    Wire.begin();
    encoderService = MagneticEncoderUpdateServiceBuilder(4, 0x70)
                        .addEncoder(onEncoderUpdate, 0)
                        .addEncoder(onEncoderUpdate, 1)
                        .build();
    encoderService->begin();
}

void loop() {
    encoderService->update();
}
```

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

#### `DifferentialRobotOdometry`
Calculates odometry for differential drive robots based on wheel velocities.

```cpp
#include <DifferentialRobotOdometry.h>

DifferentialRobotOdometry odometry(0.1, 0.5); // radius, separation

void loop() {
    // Update with current wheel speeds
    FourWheelAngularSpeed speeds = {5.0, 5.0, 5.0, 5.0};
    odometry.calculate(speeds);
    
    float v_linear = odometry.getLinearSpeed();
    float v_angular = odometry.getAngularSpeed();
}
```

---

### 🛠 Utilities

#### `WToSignedPWMConverter`
Converts physical angular limits into PWM duty cycle ranges.
* **Usage Context**: Bridges the gap between kinematic mathematics (rad/s) and the physical motors' PWM. Handles constraints such as motor deadzones (the minimum PWM required to break static friction) and maximum PWM limits to protect hardware.

```cpp
#include <WToSignedPWMConverter.h>

WToSignedPWMConverter converter(10.0, 11, 50); // maxW=10, 11-bit res, minPwm=50

void setup() {
    int pwm = converter.convert(5.0); // Converts 5.0 rad/s to PWM
}
```

#### `PersistentCounter`
Stores a simple counter value in LittleFS.

```cpp
#include <PersistentCounter.h>

PersistentCounter counter("/reboots.txt");

void setup() {
    int count = counter.read();
    counter.save(count + 1);
}
```

#### `Logger`
Simple logging utility with multiple log levels (TRACE, DEBUG, INFO, WARN, ERROR, FATAL).

```cpp
#include <Logger.h>

void setup() {
    logger.setLevel(DEBUG);
    logger.info("System initializing...");
}
```

#### `ConfigStorage`
Save and load configuration (JSON) using LittleFS.

```cpp
#include <ConfigStorage.h>

ConfigStorage config("/settings.json");

void setup() {
    config.begin();
    String ssid = config.get("wifi_ssid", "default");
    config.set("wifi_ssid", "NewSSID");
    config.save();
}
```

#### `MultiResetDetector`
Detects multiple consecutive resets to trigger special modes (e.g., WiFi config portal).

```cpp
#include <MultiResetDetector.h>

MultiResetDetector mrd(2000, 3); // 2s window, 3 resets

void setup() {
    if (mrd.detect()) {
        logger.info("Entering Config Mode...");
        // Enter config mode
    }
}

void loop() {
    mrd.process();
}
```

#### `Button`
Simple debounced button class.

```cpp
#include <Button.h>

Button btn(0); // GPIO 0 (Boot button)

void loop() {
    if (btn.pressed()) {
        // Handle press
    }
}
```

#### `SimpleTimer`
Execute tasks periodically without blocking `loop()`.
* **Usage Context**: Used across sensor nodes (e.g., IMU/GPS) to poll sensors at specific, decoupled intervals asynchronously. This avoids using `delay()` and keeps the main Arduino `loop()` running at high frequency to quickly process incoming ROS messages.

```cpp
#include <SimpleTimer.h>

// execute callback every 1000ms
SimpleTimer timer(1000, []() {
    Serial.println("Tick!");
});

void loop() {
    timer.update();
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
* **Usage Context**: Crucial for kinematics and odometry (e.g., integrating velocity to calculate position over time). Provides accurate `dt` measurement between loop iterations to ensure mathematical calculations accurately mirror physical reality.

```cpp
#include <DeltaTimeComputer.h>

DeltaTimeComputer dtComputer;

void loop() {
    dtComputer.update();
    float dt = dtComputer.deltaInMillis() / 1000.0f;
}
```

#### `I2CMultiplexor`
Switch channels on TCA9548A multiplexers.

```cpp
#include <I2CMultiplexor.h>

I2CMultiplexor mux(0x70);

void setup() {
    mux.selectChannel(2);
}
```

#### `timestamp`
NTP synchronization helper.

```cpp
#include <timestamp.h>

void setup() {
    syncClockTimeStamp(); // Syncs with pool.ntp.org
}
```

## ⚖️ License

This project is licensed under the MIT License - see the LICENSE file for details.
