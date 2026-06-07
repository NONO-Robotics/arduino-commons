# Project Instructions (GEMINI.md)

## 1. 🎯 Role & Ecosystem Context
*   **Subproject Role**: Common shared Arduino/C++ drivers and utilities library.
*   **Robot Variant**: Common to both Indoor and Outdoor robots.
*   **Ecosystem Integration**: Used as a dependency by all PlatformIO ESP32 firmware modules in the workspace to drive physical hardware and handle timing.

## 2. 🛠️ Tech Stack & Build Workflow
*   **Framework**: PlatformIO / Arduino / ESP32.
*   **Language**: C++ (Standard ISO).
*   **Build Command**: `pio run`
*   **Test Command**: `pio test` (runs unit tests on local host or ESP32 target).

## 3. ⚙️ Core Libraries & Components
*   **BLDCMotor / DCMotor**: Fluent builder patterns to easily construct and control Brushless and brushed DC motors.
*   **AS5600 / MagneticEncoder**: Driver API to read angular positions from AS5600 hall sensors over I2C (via multiplexer or directly).
*   **SimpleTimer / DeltaTimeComputer**: Lightweight non-blocking execution schedulers to prevent blocking loops.

## 4. 📜 Coding Standards & Conventions
*   **Language & Comments**: Always use **English** and Doxygen formatting (`/** @brief ... */`) for class and method interfaces.
*   **Non-Blocking Logic**: NEVER use raw `delay()` inside execution blocks. Utilize `SimpleTimer` or `DeltaTimeComputer` to allow the ESP32 micro-ROS executors to run smoothly.
*   **Memory Management**: Avoid dynamic allocation (`new`/`malloc`) inside runtime loops to prevent heap fragmentation. Use static builders and initialize during `setup()`.
