# Shared Arduino utilities

- Shared PlatformIO/Arduino/ESP32 C++ drivers and utilities; build `pio run`, test `pio test`.
- `BLDCMotor`/`DCMotor` control motors; `AS5600`/`MagneticEncoder` read I2C angle sensors; `SimpleTimer`/`DeltaTimeComputer` schedule nonblocking work.
- Use English Doxygen. No raw `delay()` or `new`/`malloc` in runtime loops; use timers and setup/static allocation.

## Workflow

- `main` is protected: no direct pushes or merges. Every change goes through a pull request.
- New code must ship with tests. `commands/regression-test` gates every push: all suites green and coverage ≥ 90% for lines, functions, and branches (gcovr, throw branches excluded).
- Tests cover coherent use cases — real behavior scenarios of the class, not assertion padding to inflate metrics.


## Pull requests only

- Every change must reach `main` through a Pull Request. This is the only permitted path: no direct pushes, no other merge route.
- Approval is always manual: open the PR, request the user's review, and stop.
- Never merge a branch into `main`.
- Never auto-approve a Pull Request.
