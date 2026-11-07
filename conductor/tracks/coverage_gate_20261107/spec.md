# Restore native branch coverage gate

## Purpose
Restore the repository's required native branch coverage of at least 90% without lowering thresholds or masking executable behavior.

## Decisions
- Preserve `GPSSensorBuilder` fail-stop behavior for missing pins or callback.
- Document only inherently non-observable defensive branches with precise gcovr exclusions and an English reason at source.
- Test reachable defensive behavior rather than excluding it.

## Requirements
1. `commands/regression-test` must pass with at least 90% lines, functions, and branches.
2. No coverage threshold, filter, or production API contract may be weakened.
3. GPS validation still logs the relevant error then never returns on embedded targets.
4. Exclusions must be limited to the non-observable non-returning failure path and impossible internal enum fallback paths; they must not hide normal execution.
5. Add coherent Unity scenarios for any reachable fallback that remains in code.

## Acceptance criteria
- [ ] Full native tests pass.
- [ ] `commands/regression-test` passes every 90% gate.
- [ ] Each exclusion has an English safety rationale at source.
- [ ] No new public API or runtime allocation is introduced.

## Out of scope
- Changing `GPSSensorBuilder` to return null.
- Reducing coverage thresholds.
- Hardware behavior changes.
