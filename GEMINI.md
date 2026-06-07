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

## 📜 Object-Oriented Programming & SOLID Standards
*   **SOLID Principles**: Strictly follow SOLID practices adapted for C++ & Arduino:
    *   `S (Single Responsibility)`: Separate hardware communication, data parsing, and ROS publishers into different classes.
    *   `O (Open/Closed)`: Favor polymorphism and abstract interfaces (e.g. abstract classes for DCMotor, IMUSensor) to allow adding new models without editing client logic.
    *   `L (Liskov Substitution)`: Subclasses (e.g. BLDCMotor) must be fully substitutable for their parent interface (DCMotor).
    *   `I (Interface Segregation)`: Maintain lightweight, cohesive interfaces focused on distinct behaviors (e.g. Updatable, Drawable).
    *   `D (Dependency Inversion)`: Inject dependencies via references or pointers to abstract classes (Dependency Injection) rather than hardcoding concrete instances.
*   **Embedded Design Patterns**: Use microcontroller-optimized design patterns:
    *   `Fluent Builder`: To cleanly configure and initialize hardware modules (e.g. BLDCMotorBuilder) without bloated constructors.
    *   `Strategy`: Decouple control algorithms (e.g. Mecanum vs Differential Kinematics) from physical actuator drivers.
    *   `Observer / Callback`: Use non-blocking events and function pointers/lambdas for asynchronous ROS subscription and polling tasks.
