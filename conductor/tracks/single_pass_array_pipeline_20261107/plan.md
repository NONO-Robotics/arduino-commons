# Implementation Plan: Single-pass C++17 array pipeline

## Phase 1: Lazy pipeline foundation

- [x] Task: Add failing Unity scenarios for array source views, lazy construction, filter/map ordering, empty arrays, and a single `forEach` terminal traversal.
- [x] Task: Implement header-only source views and typed lazy `filter`/`map` composition in C++17 without heap allocation, type erasure, or virtual dispatch.
- [x] Task: Expose the public pipeline header through `ArduinoCommons.h` and make terminal consumption rvalue-qualified.
- [x] Task: Conductor - User Manual Verification 'Lazy pipeline foundation' (Protocol in workflow.md) — verified: pio test 31 suites, 124/124; manual review approved.

## Phase 2: Terminal operations

- [x] Task: Add failing Unity scenarios for `find`, `any`, `all`, `count`, `reduce`, and bounded `collectInto`, including short-circuit and truncation behavior.
- [x] Task: Implement terminal operations with a single source traversal, direct callback dispatch, and the specified null/empty input policy.
- [x] Task: Conductor - User Manual Verification 'Terminal operations' (Protocol in workflow.md) — verified: pio test 31 suites, 130/130; manual review approved.

## Phase 3: Quality evidence

- [x] Task: Add Unity coverage for compile-time single-use semantics, pointer-plus-length subranges, mapper type changes, and error paths.
- [x] Task: Update README usage examples and English Doxygen for every public pipeline API; run `commands/docs` with zero warnings. Generated `docs/` is output, never durable authored content.
- [x] Task: Run `pio test` and `commands/regression-test`; maintain at least 90% line, function, and branch coverage.
- [x] Task: Conductor - User Manual Verification 'Quality evidence' (Protocol in workflow.md) — verified: Doxygen clean; pio test 31 suites, 134/134; regression 99.9% lines, 100% functions, 90.4% branches; manual review approved.
