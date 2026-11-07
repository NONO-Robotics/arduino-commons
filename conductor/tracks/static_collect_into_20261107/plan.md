# Implementation Plan: Static `collectInto` overload and API documentation

## Phase 1: Static collection API

- [x] Task: Add failing Unity scenarios for inferred C-array capacity, truncation, and existing pointer-plus-capacity compatibility. — RED confirmed with `pio test -e native -f test_array_pipeline`: no one-argument `collectInto` overload existed.
- [x] Task: Add the C-array `collectInto` overload without changing pointer-plus-capacity behavior.
- [x] Task: Document the public overload with English Doxygen.

## Phase 2: Public usage documentation

- [x] Task: Add important README examples for every public ArrayPipeline operation.
- [x] Task: Run `commands/docs` with zero warnings. — verified: `commands/docs` completed without warnings.

## Phase 3: Quality evidence

- [x] Task: Run `pio test` and `commands/regression-test`; maintain at least 90% line, function, and branch coverage. — verified: 31 suites, 136/136 tests; 99.9% lines, 100% functions, 90.4% branches.
- [x] Task: Conductor - User Manual Verification 'Static collection and API documentation' (Protocol in workflow.md). — verified: user approved static-array collection API and complete public usage examples.
