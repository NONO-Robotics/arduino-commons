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

* [4w-ros-robot](https://github.com/adrianmarino/4w-ros-robot)
  * **Navigation**
    * [4w-robot-ros-ws](https://github.com/adrianmarino/4w-robot-ros-ws): Autonomous/manual navigation control project.
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
      * [4w-robot-ros-movement](https://github.com/adrianmarino/4w-robot-ros-movement.git): Movement controller firmware.
    * **Sensor data publisher firmware**
      * [4w-robot-ros-w-publisher](https://github.com/adrianmarino/4w-robot-ros-w-publisher): Wheels angular velocity sensors publisher firmware.
      * [4w-robot-ros-imu-gps](https://github.com/adrianmarino/4w-ros-robot-imu-gps): IMU, GPS sensors publisher firmware.
  * **Outdoor**
    * **Navigation**
      * [4w-outdoor-robot-ros-movement](https://github.com/adrianmarino/4w-outdoor-robot-ros-movement.git): Outdoor Movement controller firmware.
    * **Sensor data publisher firmware**
      * [4w-outdoor-robot-ros-w-publisher](https://github.com/adrianmarino/4w-outdoor-robot-ros-w-publisher): Outdoor wheels angular velocity sensors publisher firmware.


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
