# Single-pass array pipeline

## Purpose

Provide a C++17, header-only fluent API for fixed-size C arrays and pointer-plus-length arrays. It composes filtering and transformation lazily, then consumes the source with exactly one source-loop when a terminal operation runs.

## Scope

- Input: `T (&array)[N]` and `T* data, size_t length`.
- Intermediate operations: `filter(predicate)` and `map(mapper)`.
- Terminal operations: `forEach`, `find`, `any`, `all`, `count`, `reduce`, and `collectInto`.
- Terminal methods are the sole shortcut API: no duplicate free functions; `filter` and `map` require a terminal method because they have no observable result.
- Supported target: PlatformIO native tests and ESP32 firmware; no heap allocation, virtual dispatch, `std::function`, containers, exceptions, or RTTI requirements.
- Public entry point exposed through `ArduinoCommons.h`.

## API sketch

```cpp
int readings[] = {1, 2, 3, 4};

arrayPipeline(readings)
    .filter([](int value) { return value % 2 == 0; })
    .map([](int value) { return value * 10; })
    .forEach([](int value) { /* receives 20 then 40 */ });
```

```cpp
int output[2]{};
const CollectResult collected = arrayPipeline(readings)
    .filter([](int value) { return value % 2 == 0; })
    .map([](int value) { return value * 10; })
    .collectInto(output, 2); // {20, 40}; written=2; truncated=false

int firstEven{};
const bool found = arrayPipeline(readings)
    .find([](int value) { return value % 2 == 0; }, firstEven); // true; firstEven=2

const int sum = arrayPipeline(readings)
    .filter([](int value) { return value % 2 == 0; })
    .reduce(0, [](int total, int value) { return total + value; }); // 6
```

The final names may change only for collision or project-style reasons. This sketch defines behavior, not a required implementation.

## Functional requirements

1. Pipeline construction performs no traversal and no user-callback invocation.
2. Every terminal operation performs at most one `for` loop over the source array, from index `0` to `length - 1`.
3. Each source element reaches stages in declaration order.
4. A filter invokes its predicate at most once per source element. Rejected elements do not reach subsequent stages.
5. A map invokes its mapper exactly once for each element accepted by all prior filters. Its result feeds the following stage or terminal consumer.
6. Multiple filters and maps may be arbitrarily interleaved.
7. Empty arrays complete without invoking predicates, mappers, or consumers.
8. Source elements are observed as `const T&`; the pipeline does not mutate, copy, or own the source array.
9. Mappers may change the element type. Mapper results are passed directly to the next stage; references returned by a mapper must remain valid through that callback chain.
10. `forEach(consumer)` invokes `consumer` for each output element and returns `void`.
11. `find(predicate, output)` evaluates output elements in order, assigns the first matching value to caller-provided `output`, returns `true`, and stops immediately. It returns `false` without assigning `output` when no value matches.
12. `any(predicate)` returns `true` and stops at the first matching output element; `all(predicate)` returns `false` and stops at the first non-matching output element. `all` returns `true` for an empty output sequence.
13. `count()` returns the number of output elements after all intermediate stages.
14. `reduce(initial, reducer)` invokes `reducer(accumulator, value)` once per output element and returns the final accumulator. It returns `initial` for an empty output sequence.
15. `collectInto(output, capacity)` copies output elements in order to caller-provided writable storage. It returns `CollectResult { size_t written, bool truncated }`. When capacity is exhausted, it continues traversing the source without writing further values so `truncated` accurately indicates discarded output; it never writes out of bounds.
16. A pipeline is single-use: terminal operations consume an rvalue pipeline. Re-running a stored pipeline must require an explicit new pipeline construction rather than silently performing another pass.

## Performance requirements

1. Exactly one source traversal per terminal invocation. No intermediate arrays or buffers.
2. All stage types and callbacks are template parameters, enabling compile-time composition and inlining under optimized ESP32 builds.
3. The implementation must not allocate dynamically, use type erasure, virtual dispatch, or `std::function`.
4. Pipeline object memory is bounded by the stored source view plus callback objects; lambda captures determine additional size.
5. `find`, `any`, and `all` may stop before the final source element. Other terminals inspect every source element once.

## Validation and errors

- `data == nullptr` with `length > 0` is invalid. Define and test the chosen policy: debug assertion or safe no-op. Prefer assertion because this is a programmer error.
- `nullptr` with `length == 0` is valid.
- Predicate must be callable with the current element and return a boolean-convertible value.
- Mapper and consumer must be callable with the current element. Invalid callbacks should fail at compile time with focused `static_assert` diagnostics where practical in C++17.

## Implementation plan

1. Add a self-contained public header for the pipeline and include it from `ArduinoCommons.h`.
2. Model the source as a non-owning `data + length` view.
3. Represent each `filter` and `map` as a typed, lazy stage stored by value. Compose stages through templates, not runtime collections.
4. Implement terminal operations around the same composed stage dispatch: `forEach`, `find`, `any`, `all`, `count`, `reduce`, and bounded `collectInto`.
5. Make terminal methods rvalue-qualified to encode single-use semantics at compile time.
6. Avoid a `.cpp` file unless concrete non-template logic proves necessary.

## Tests

Create one Unity suite covering coherent behaviors:

- Filter then map then consume yields expected values and order.
- Interleaved filters/maps preserve declaration order.
- Counters prove one source loop: predicate count equals source length; mapper and consumer counts equal accepted elements.
- No callback runs until `forEach`.
- Empty arrays invoke no callbacks.
- Type-changing map compiles and produces expected consumer values.
- Pointer-plus-length input handles a valid subrange.
- Null-empty input is valid; invalid non-empty null input follows the specified assertion/no-op policy.
- `find`, `any`, and `all` stop at their first decisive element; no later callback is invoked.
- `count` and `reduce` produce expected values, including empty output sequences.
- `collectInto` preserves output order, reports `written`, and truncates safely without out-of-bounds writes.
- Compile-time check that an lvalue pipeline cannot be consumed twice.

Run `pio test` and the repository regression command. Preserve the repository threshold: at least 90% lines, functions, and branches.

## Documentation requirements

Because this implementation introduces public APIs, update README examples/usage and write English Doxygen for every public API. Run `commands/docs` with zero warnings. Generated `docs/` is output, never durable authored content.

## Out of scope

- Dynamic containers, iterators, generators, coroutines, parallel execution, sorting, and arbitrary terminal operations beyond the defined set.
- General-purpose replacement for Kotlin sequences, Java streams, or Smalltalk collections.
- ESP32 benchmark execution; it is not required for the current track.

## Acceptance checklist

- [ ] Fluent `filter().map().forEach()` works over C arrays.
- [ ] A terminal operation traverses the source exactly once.
- [ ] No heap allocation, virtual dispatch, or type erasure exists in the pipeline path.
- [ ] Single-use behavior is compile-time enforced.
- [ ] Unity tests and coverage gate pass.
