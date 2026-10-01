# Shared Arduino utilities

- Shared PlatformIO/Arduino/ESP32 C++ drivers and utilities; build `pio run`, test `pio test`.
- `BLDCMotor`/`DCMotor` control motors; `AS5600`/`MagneticEncoder` read I2C angle sensors; `SimpleTimer`/`DeltaTimeComputer` schedule nonblocking work.
- Use English Doxygen. No raw `delay()` or `new`/`malloc` in runtime loops; use timers and setup/static allocation.
