# Static `collectInto` overload and API documentation

## Purpose

Allow callers to collect a pipeline into a caller-owned C array without repeating its capacity, while documenting practical use for every public `ArrayPipeline` operation.

## Scope

- Add `collectInto(Output (&output)[N])` as an rvalue terminal operation.
- Delegate its behavior to existing `collectInto(Output*, std::size_t)`.
- Preserve the pointer-and-capacity overload for dynamically sized views.
- Preserve one source traversal, bounded writes, and zero heap allocation.
- Add README examples for `arrayPipeline`, `filter`, `map`, and every terminal operation.
- Add English Doxygen for the static-array overload.

## Functional requirements

1. `collectInto(output)` infers `N` from a C-array argument and returns `CollectResult`.
2. Its output ordering, `written`, and `truncated` behavior match `collectInto(output, N)`.
3. A smaller output array truncates safely while still consuming all accepted source values.
4. The pointer-and-capacity overload remains callable and behaviorally unchanged.
5. No dynamic allocation, additional source pass, type erasure, or virtual dispatch is introduced.

## Tests

- Static-array collection infers capacity, preserves filtered/mapped order, and reports completion.
- Static-array collection truncates safely and reports truncation.
- Pointer-and-capacity collection remains compatible.

## Documentation requirements

- README contains important, runnable-style examples for source creation, `filter`, `map`, `forEach`, `find`, `any`, `all`, `count`, `reduce`, and both `collectInto` forms.
- Public overload is documented with English Doxygen.

## Out of scope

- Heap-allocating `collect()`.
- `do()` or `drain()` terminal methods.
- Non-C-array containers and iterator support.
