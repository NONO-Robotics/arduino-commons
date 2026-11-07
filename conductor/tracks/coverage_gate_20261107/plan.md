# Implementation Plan: Restore native branch coverage gate

## Phase 1: Defensive branch classification

- [x] Task: Add Unity coverage for reachable invalid wheel-position fallback behavior.
- [x] Task: Classify untestable non-returning GPS validation and impossible internal enum fallback paths against gcovr exclusion rules.
- [x] Task: Conductor - User Manual Verification 'Defensive branch classification' (Protocol in workflow.md) — verified: pio test 31 suites, 134/134; manual review approved.

## Phase 2: Coverage gate recovery

- [x] Task: Add minimal, source-local gcovr exclusions with English safety rationales only for approved non-observable defensive paths.
- [x] Task: Run `pio test` and `commands/regression-test`; preserve at least 90% coverage for lines, functions, and branches.
- [x] Task: Conductor - User Manual Verification 'Coverage gate recovery' (Protocol in workflow.md) — verified: pio test 31 suites, 134/134; regression 99.9% lines, 100% functions, 90.4% branches; manual review approved.
