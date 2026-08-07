# Project Instructions (AGENTS.md)

## 1. 🎯 Role & Ecosystem Context
*   **Subproject Role**: Common shared Arduino/C++ drivers + utilities library.
*   **Robot Variant**: Common — Indoor + Outdoor robots.
*   **Ecosystem Integration**: Dependency for all PlatformIO ESP32 firmware modules — drives physical hardware + handles timing.

## 2. 🛠️ Tech Stack & Build Workflow
*   **Framework**: PlatformIO / Arduino / ESP32.
*   **Language**: C++ (ISO).
*   **Build**: `pio run`
*   **Test**: `pio test` (unit tests on local host or ESP32 target).

## 3. ⚙️ Core Libraries & Components
*   **BLDCMotor / DCMotor**: Fluent builder patterns for brushless + brushed DC motor construction/control.
*   **AS5600 / MagneticEncoder**: Read angular positions from AS5600 hall sensors over I2C (multiplexer or direct).
*   **SimpleTimer / DeltaTimeComputer**: Lightweight non-blocking execution schedulers — no blocking loops.

## 4. 📜 Coding Standards & Conventions
*   **Language & Comments**: English + Doxygen (`/** @brief ... */`) for class/method interfaces.
*   **Non-Blocking Logic**: NEVER raw `delay()`. Use `SimpleTimer` or `DeltaTimeComputer` — ESP32 micro-ROS executors run smoothly.
*   **Memory Management**: No dynamic allocation (`new`/`malloc`) in runtime loops — prevent heap fragmentation. Static builders + `setup()`.

## 📜 Object-Oriented Programming & SOLID Standards
*   **SOLID Principles**: Strict C++ & Arduino SOLID practices:
    *   `S (SRP)`: Separate hardware communication, data parsing, ROS publishers.
    *   `O (OCP)`: Polymorphism/abstract interfaces (e.g. `DCMotor`, `IMUSensor`) — add new models without editing client logic.
    *   `L (LSP)`: Subclasses (e.g. `BLDCMotor`) fully substitutable for parent interface (`DCMotor`).
    *   `I (ISP)`: Lightweight cohesive interfaces: `Updatable`, `Drawable`.
    *   `D (DIP)`: Inject dependencies via references/pointers to abstract classes, not hardcoded instances.
*   **Embedded Design Patterns**:
    *   `Fluent Builder`: Configure hardware modules cleanly (e.g. `BLDCMotorBuilder`), no bloated constructors.
    *   `Strategy`: Decouple control algorithms (Mecanum vs Differential) from actuator drivers.
    *   `Observer / Callback`: Non-blocking events, function pointers/lambdas for async ROS subscription/polling.
