#pragma once

#include <cassert>
#include <cstddef>
#include <type_traits>
#include <utility>

/**
 * @brief Result of a bounded pipeline collection.
 */
struct CollectResult {
  /** Number of accepted values written to the output buffer. */
  std::size_t written;

  /** True when accepted values exceeded the output buffer capacity. */
  bool truncated;
};

namespace array_pipeline_detail {

template <typename Source, typename Predicate>
class FilterPipeline;

template <typename Source, typename Mapper>
class MapPipeline;

template <typename Derived>
class PipelineOperations {
 public:
  /**
   * @brief Appends a lazy predicate stage to the pipeline.
   * @tparam Predicate Callable invoked with each source value.
   * @param predicate Callable that returns true for values to retain.
   * @return A pipeline that evaluates @p predicate during terminal consumption.
   */
  template <typename Predicate>
  auto filter(Predicate predicate) && {
    return FilterPipeline<Derived, typename std::decay<Predicate>::type>(
        std::move(static_cast<Derived&>(*this)), std::move(predicate));
  }

  /**
   * @brief Appends a lazy transformation stage to the pipeline.
   * @tparam Mapper Callable invoked with each value accepted by prior stages.
   * @param mapper Callable that transforms an accepted value.
   * @return A pipeline that evaluates @p mapper during terminal consumption.
   */
  template <typename Mapper>
  auto map(Mapper mapper) && {
    return MapPipeline<Derived, typename std::decay<Mapper>::type>(
        std::move(static_cast<Derived&>(*this)), std::move(mapper));
  }

  /**
   * @brief Consumes every accepted pipeline value.
   * @tparam Consumer Callable invoked once for each accepted value.
   * @param consumer Callable receiving each accepted value by const reference.
   * @note Traverses the complete source because the consumer cannot stop it.
   */
  template <typename Consumer>
  void forEach(Consumer consumer) && {
    std::move(static_cast<Derived&>(*this)).dispatch(
        [consumer = std::move(consumer)](const auto& value) mutable {
          consumer(value);
          return true;
        });
  }

  /**
   * @brief Finds the first accepted value that matches a predicate.
   * @tparam Predicate Callable returning true for the sought value.
   * @tparam Output Assignable destination type.
   * @param predicate Callable evaluated for each accepted value.
   * @param output Destination assigned the first matching value.
   * @return True after assigning @p output; false when no value matches.
   * @note Stops source traversal at the first matching value.
   */
  template <typename Predicate, typename Output>
  bool find(Predicate predicate, Output& output) && {
    bool found = false;
    std::move(static_cast<Derived&>(*this)).dispatch(
        [&predicate, &output, &found](const auto& value) {
          if (predicate(value)) {
            output = value;
            found = true;
            return false;
          }
          return true;
        });
    return found;
  }

  /**
   * @brief Tests whether any accepted value matches a predicate.
   * @tparam Predicate Callable returning true for a matching value.
   * @param predicate Callable evaluated for each accepted value.
   * @return True when a value matches; false when no accepted value matches.
   * @note Stops source traversal at the first matching value.
   */
  template <typename Predicate>
  bool any(Predicate predicate) && {
    bool matches = false;
    std::move(static_cast<Derived&>(*this)).dispatch(
        [&predicate, &matches](const auto& value) {
          matches = predicate(value);
          return !matches;
        });
    return matches;
  }

  /**
   * @brief Tests whether every accepted value matches a predicate.
   * @tparam Predicate Callable returning true for a matching value.
   * @param predicate Callable evaluated for each accepted value.
   * @return True when every accepted value matches, including an empty pipeline.
   * @note Stops source traversal at the first non-matching value.
   */
  template <typename Predicate>
  bool all(Predicate predicate) && {
    bool matches = true;
    std::move(static_cast<Derived&>(*this)).dispatch(
        [&predicate, &matches](const auto& value) {
          matches = predicate(value);
          return matches;
        });
    return matches;
  }

  /**
   * @brief Counts accepted pipeline values.
   * @return Number of values that reach this terminal operation.
   * @note Traverses the complete source.
   */
  std::size_t count() && {
    std::size_t total = 0;
    std::move(static_cast<Derived&>(*this)).dispatch(
        [&total](const auto&) {
          ++total;
          return true;
        });
    return total;
  }

  /**
   * @brief Reduces accepted values into an accumulator.
   * @tparam Accumulator Value type carried between reducer calls.
   * @tparam Reducer Callable accepting the accumulator and an accepted value.
   * @param initial Initial accumulator value.
   * @param reducer Callable returning the next accumulator value.
   * @return The final accumulator, or @p initial when no values are accepted.
   * @note Traverses the complete source.
   */
  template <typename Accumulator, typename Reducer>
  Accumulator reduce(Accumulator initial, Reducer reducer) && {
    std::move(static_cast<Derived&>(*this)).dispatch(
        [&initial, &reducer](const auto& value) {
          initial = reducer(initial, value);
          return true;
        });
    return initial;
  }

  /**
   * @brief Collects accepted values into a bounded output buffer.
   * @tparam Output Element type of the caller-owned output buffer.
   * @param output Destination buffer, or null only when @p capacity is zero.
   * @param capacity Number of writable elements in @p output.
   * @return Count of written values and whether later accepted values were truncated.
   * @note Traverses the complete source to determine whether truncation occurred.
   */
  template <typename Output>
  CollectResult collectInto(Output* output, std::size_t capacity) && {
    assert(output != nullptr || capacity == 0);
    CollectResult result{0, false};
    std::move(static_cast<Derived&>(*this)).dispatch(
        [&output, capacity, &result](const auto& value) {
          if (result.written < capacity) {
            output[result.written++] = value;
          } else {
            result.truncated = true;
          }
          return true;
        });
    return result;
  }

  /**
   * @brief Collects accepted values into a caller-owned C array.
   * @tparam Output Element type of the caller-owned output array.
   * @tparam Capacity Compile-time number of writable elements in @p output.
   * @param output Destination array whose capacity is inferred automatically.
   * @return Count of written values and whether later accepted values were truncated.
   * @note Delegates to the pointer-and-capacity overload and traverses the source once.
   */
  template <typename Output, std::size_t Capacity>
  CollectResult collectInto(Output (&output)[Capacity]) && {
    return std::move(static_cast<Derived&>(*this)).collectInto(output, Capacity);
  }
};

template <typename Consumer, typename Predicate>
class FilterDispatcher {
 public:
  /**
   * @brief Stores a consumer and predicate for lazy filtered dispatch.
   * @param consumer Callable receiving values that satisfy @p predicate.
   * @param predicate Callable deciding whether to forward each source value.
   */
  FilterDispatcher(Consumer consumer, Predicate predicate)
      : consumer_(std::move(consumer)), predicate_(std::move(predicate)) {}

  /**
   * @brief Forwards a value only when its predicate matches.
   * @tparam Value Source value type.
   * @param value Source value to test.
   * @return The consumer continuation result, or true when the value is filtered out.
   */
  template <typename Value>
  bool operator()(const Value& value) {
    if (predicate_(value)) {
      return consumer_(value);
    }
    return true;
  }

 private:
  Consumer consumer_;
  Predicate predicate_;
};

template <typename Consumer, typename Mapper>
class MapDispatcher {
 public:
  /**
   * @brief Stores a consumer and mapper for lazy mapped dispatch.
   * @param consumer Callable receiving mapped source values.
   * @param mapper Callable transforming each source value.
   */
  MapDispatcher(Consumer consumer, Mapper mapper)
      : consumer_(std::move(consumer)), mapper_(std::move(mapper)) {}

  /**
   * @brief Maps a source value and forwards it to the consumer.
   * @tparam Value Source value type.
   * @param value Source value to map.
   * @return The consumer continuation result.
   */
  template <typename Value>
  bool operator()(const Value& value) {
    return consumer_(mapper_(value));
  }

 private:
  Consumer consumer_;
  Mapper mapper_;
};

template <typename Source, typename Predicate>
class FilterPipeline
    : public PipelineOperations<FilterPipeline<Source, Predicate>> {
 public:
  /**
   * @brief Stores a source pipeline and predicate stage.
   * @param source Pipeline consumed when this stage reaches a terminal operation.
   * @param predicate Callable retaining matching source values.
   */
  FilterPipeline(Source source, Predicate predicate)
      : source_(std::move(source)), predicate_(std::move(predicate)) {}

  /**
   * @brief Dispatches values accepted by this filter stage.
   * @tparam Consumer Callable returning true to continue or false to stop.
   * @param consumer Continuation invoked for each accepted value.
   * @note Propagates early termination from @p consumer to the source pipeline.
   */
  template <typename Consumer>
  void dispatch(Consumer consumer) && {
    std::move(source_).dispatch(
        FilterDispatcher<Consumer, Predicate>(std::move(consumer), std::move(predicate_)));
  }

 private:
  Source source_;
  Predicate predicate_;
};

template <typename Source, typename Mapper>
class MapPipeline : public PipelineOperations<MapPipeline<Source, Mapper>> {
 public:
  /**
   * @brief Stores a source pipeline and mapper stage.
   * @param source Pipeline consumed when this stage reaches a terminal operation.
   * @param mapper Callable transforming accepted source values.
   */
  MapPipeline(Source source, Mapper mapper)
      : source_(std::move(source)), mapper_(std::move(mapper)) {}

  /**
   * @brief Dispatches mapped values from this stage.
   * @tparam Consumer Callable returning true to continue or false to stop.
   * @param consumer Continuation invoked for each mapped value.
   * @note Propagates early termination from @p consumer to the source pipeline.
   */
  template <typename Consumer>
  void dispatch(Consumer consumer) && {
    std::move(source_).dispatch(
        MapDispatcher<Consumer, Mapper>(std::move(consumer), std::move(mapper_)));
  }

 private:
  Source source_;
  Mapper mapper_;
};

}  // namespace array_pipeline_detail

/**
 * @brief Lazily transforms a non-owning, immutable array view.
 * @tparam T Source element type.
 *
 * Create instances with arrayPipeline(), compose filter() and map(), then
 * consume the result once with a terminal operation.
 */
template <typename T>
class ArrayPipeline : public array_pipeline_detail::PipelineOperations<ArrayPipeline<T>> {
 public:
  /**
   * @brief Creates a non-owning immutable array view.
   * @param data Source data, or null when @p length is zero.
   * @param length Number of source elements.
   */
  ArrayPipeline(const T* data, std::size_t length) : data_(data), length_(length) {
    assert(data != nullptr || length == 0);
  }

  /**
   * @brief Dispatches source values to a continuation.
   * @tparam Consumer Callable returning true to continue or false to stop.
   * @param consumer Continuation invoked with each source value by const reference.
   * @note Stops source traversal when @p consumer returns false.
   */
  template <typename Consumer>
  void dispatch(Consumer consumer) && {
    for (std::size_t index = 0; index < length_; ++index) {
      if (!consumer(data_[index])) {
        break;
      }
    }
  }

 private:
  const T* data_;
  std::size_t length_;
};

/**
 * @brief Creates a lazy pipeline over a fixed-size array.
 * @tparam T Array element type.
 * @tparam N Number of elements in @p data.
 * @param data Source array retained by the caller.
 * @return A non-owning pipeline view.
 */
template <typename T, std::size_t N>
ArrayPipeline<T> arrayPipeline(const T (&data)[N]) {
  return ArrayPipeline<T>(data, N);
}

/**
 * @brief Creates a lazy pipeline over an array view.
 * @tparam T Array element type.
 * @param data Source data retained by the caller, or null only when @p length is zero.
 * @param length Number of elements in @p data.
 * @return A non-owning pipeline view.
 */
template <typename T>
ArrayPipeline<T> arrayPipeline(const T* data, std::size_t length) {
  return ArrayPipeline<T>(data, length);
}
