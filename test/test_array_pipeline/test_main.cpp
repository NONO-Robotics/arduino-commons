#include <type_traits>
#include <utility>

#include <unity.h>

#include "ArduinoCommons.h"

struct ForEachIntConsumer {
  void operator()(int) const {}
};

template <typename Type, typename = void>
struct CanCallForEach : std::false_type {};

template <typename Type>
struct CanCallForEach<
    Type, std::void_t<decltype(std::declval<Type>().forEach(ForEachIntConsumer{}))>>
    : std::true_type {};

static_assert(!CanCallForEach<ArrayPipeline<int>&>::value,
              "forEach must require an rvalue pipeline");

void test_array_pipeline_defers_filtering_and_mapping_until_for_each() {
  const int source[] = {1, 2, 3};
  int predicateCalls = 0;
  int mapperCalls = 0;

  auto pipeline = arrayPipeline(source)
                      .filter([&predicateCalls](int value) {
                        ++predicateCalls;
                        return value % 2 == 1;
                      })
                      .map([&mapperCalls](int value) {
                        ++mapperCalls;
                        return value * 10;
                      });

  TEST_ASSERT_EQUAL_INT(0, predicateCalls);
  TEST_ASSERT_EQUAL_INT(0, mapperCalls);

  std::move(pipeline).forEach([](int) {});

  TEST_ASSERT_EQUAL_INT(3, predicateCalls);
  TEST_ASSERT_EQUAL_INT(2, mapperCalls);
}

void test_array_pipeline_filters_before_mapping_and_preserves_order() {
  const int source[] = {1, 2, 3};
  int callbackOrder[5] = {};
  int output[2] = {};
  int callbackCount = 0;
  int outputCount = 0;

  std::move(arrayPipeline(source)
                .filter([&callbackOrder, &callbackCount](int value) {
                  callbackOrder[callbackCount++] = 100 + value;
                  return value % 2 == 1;
                })
                .map([&callbackOrder, &callbackCount](int value) {
                  callbackOrder[callbackCount++] = value;
                  return value * 10;
                }))
      .forEach([&output, &outputCount](int value) {
        output[outputCount++] = value;
      });

  TEST_ASSERT_EQUAL_INT(5, callbackCount);
  TEST_ASSERT_EQUAL_INT(101, callbackOrder[0]);
  TEST_ASSERT_EQUAL_INT(1, callbackOrder[1]);
  TEST_ASSERT_EQUAL_INT(102, callbackOrder[2]);
  TEST_ASSERT_EQUAL_INT(103, callbackOrder[3]);
  TEST_ASSERT_EQUAL_INT(3, callbackOrder[4]);
  TEST_ASSERT_EQUAL_INT(2, outputCount);
  TEST_ASSERT_EQUAL_INT(10, output[0]);
  TEST_ASSERT_EQUAL_INT(30, output[1]);
}

void test_array_pipeline_empty_source_invokes_no_callbacks() {
  const int* source = nullptr;
  int predicateCalls = 0;
  int mapperCalls = 0;
  int consumerCalls = 0;

  std::move(arrayPipeline(source, 0)
                .filter([&predicateCalls](int) {
                  ++predicateCalls;
                  return true;
                })
                .map([&mapperCalls](int value) {
                  ++mapperCalls;
                  return value;
                }))
      .forEach([&consumerCalls](int) {
        ++consumerCalls;
      });

  TEST_ASSERT_EQUAL_INT(0, predicateCalls);
  TEST_ASSERT_EQUAL_INT(0, mapperCalls);
  TEST_ASSERT_EQUAL_INT(0, consumerCalls);
}

void test_array_pipeline_invokes_callbacks_for_accepted_elements_only() {
  const int source[] = {1, 2, 3, 4};
  int predicateCalls = 0;
  int mapperCalls = 0;
  int consumerCalls = 0;

  std::move(arrayPipeline(source)
                .filter([&predicateCalls](int value) {
                  ++predicateCalls;
                  return value % 2 == 0;
                })
                .map([&mapperCalls](int value) {
                  ++mapperCalls;
                  return value * 10;
                }))
      .forEach([&consumerCalls](int) {
        ++consumerCalls;
      });

  TEST_ASSERT_EQUAL_INT(4, predicateCalls);
  TEST_ASSERT_EQUAL_INT(2, mapperCalls);
  TEST_ASSERT_EQUAL_INT(2, consumerCalls);
}

void test_array_pipeline_for_each_without_filter_observes_all_elements_in_order() {
  const int source[] = {4, 2, 8};
  int output[3] = {};
  int outputCount = 0;

  auto pipeline = arrayPipeline(source);
  std::move(pipeline).forEach([&output, &outputCount](int value) {
    output[outputCount++] = value;
  });

  TEST_ASSERT_EQUAL_INT(3, outputCount);
  TEST_ASSERT_EQUAL_INT(4, output[0]);
  TEST_ASSERT_EQUAL_INT(2, output[1]);
  TEST_ASSERT_EQUAL_INT(8, output[2]);
}

void test_array_pipeline_pointer_length_processes_selected_subrange_in_order() {
  const int source[] = {1, 2, 3, 4, 5};
  int output[3] = {};
  int outputCount = 0;

  std::move(arrayPipeline(source + 1, 3)).forEach(
      [&output, &outputCount](int value) { output[outputCount++] = value; });

  TEST_ASSERT_EQUAL_INT(3, outputCount);
  TEST_ASSERT_EQUAL_INT(2, output[0]);
  TEST_ASSERT_EQUAL_INT(3, output[1]);
  TEST_ASSERT_EQUAL_INT(4, output[2]);
}

void test_array_pipeline_mapper_type_change_reaches_for_each() {
  const int source[] = {1, 2, 3};
  float output[3] = {};
  int outputCount = 0;

  std::move(arrayPipeline(source).map([](int value) {
    return static_cast<float>(value) / 2.0F;
  })).forEach([&output, &outputCount](float value) {
    output[outputCount++] = value;
  });

  TEST_ASSERT_EQUAL_INT(3, outputCount);
  TEST_ASSERT_FLOAT_WITHIN(0.001F, 0.5F, output[0]);
  TEST_ASSERT_FLOAT_WITHIN(0.001F, 1.0F, output[1]);
  TEST_ASSERT_FLOAT_WITHIN(0.001F, 1.5F, output[2]);
}

void test_array_pipeline_find_returns_first_transformed_match_and_short_circuits() {
  const int source[] = {1, 2, 3, 4, 5};
  int filterCalls = 0;
  int mapperCalls = 0;
  int found = 0;

  const bool wasFound = std::move(arrayPipeline(source)
                                      .filter([&filterCalls](int value) {
                                        ++filterCalls;
                                        return value % 2 == 1;
                                      })
                                      .map([&mapperCalls](int value) {
                                        ++mapperCalls;
                                        return value * 10;
                                      }))
                            .find([](int value) { return value >= 30; }, found);

  TEST_ASSERT_TRUE(wasFound);
  TEST_ASSERT_EQUAL_INT(30, found);
  TEST_ASSERT_EQUAL_INT(3, filterCalls);
  TEST_ASSERT_EQUAL_INT(2, mapperCalls);
}

void test_array_pipeline_find_preserves_output_when_no_element_matches() {
  const int source[] = {1, 2, 3};
  int found = 99;

  const bool wasFound = std::move(arrayPipeline(source).map([](int value) {
    return value * 10;
  })).find([](int value) { return value > 50; }, found);

  TEST_ASSERT_FALSE(wasFound);
  TEST_ASSERT_EQUAL_INT(99, found);
}

void test_array_pipeline_any_returns_on_first_match() {
  const int source[] = {1, 2, 3, 4};
  int predicateCalls = 0;

  const bool hasMatch = std::move(arrayPipeline(source)).any(
      [&predicateCalls](int value) {
        ++predicateCalls;
        return value == 2;
      });

  TEST_ASSERT_TRUE(hasMatch);
  TEST_ASSERT_EQUAL_INT(2, predicateCalls);
}

void test_array_pipeline_all_stops_at_first_non_match_and_empty_is_true() {
  const int source[] = {2, 4, 3, 6};
  const int* emptySource = nullptr;
  int predicateCalls = 0;

  const bool allEven = std::move(arrayPipeline(source)).all(
      [&predicateCalls](int value) {
        ++predicateCalls;
        return value % 2 == 0;
      });
  const bool emptyIsAllEven = std::move(arrayPipeline(emptySource, 0)).all(
      [](int value) { return value % 2 == 0; });

  TEST_ASSERT_FALSE(allEven);
  TEST_ASSERT_EQUAL_INT(3, predicateCalls);
  TEST_ASSERT_TRUE(emptyIsAllEven);
}

void test_array_pipeline_count_and_reduce_consume_pipeline_output() {
  const int source[] = {1, 2, 3, 4};
  const int emptySource[1] = {};

  const size_t count = std::move(arrayPipeline(source).filter([](int value) {
    return value % 2 == 0;
  })).count();
  const int sum = std::move(arrayPipeline(source).map([](int value) {
    return value * 10;
  })).reduce(5, [](int accumulator, int value) {
    return accumulator + value;
  });
  const int initial = std::move(arrayPipeline(emptySource, 0)).reduce(
      5, [](int accumulator, int value) { return accumulator + value; });

  TEST_ASSERT_EQUAL_UINT(2, count);
  TEST_ASSERT_EQUAL_INT(105, sum);
  TEST_ASSERT_EQUAL_INT(5, initial);
}

void test_array_pipeline_collect_into_preserves_order_and_reports_capacity() {
  const int source[] = {1, 2, 3, 4};
  int completeOutput[2] = {0, 0};
  int truncatedOutput[2] = {0, 77};
  int filterCalls = 0;
  int mapperCalls = 0;

  const auto completeResult = std::move(arrayPipeline(source)
                                            .filter([](int value) {
                                              return value % 2 == 0;
                                            })
                                            .map([](int value) {
                                              return value * 10;
                                            }))
                                  .collectInto(completeOutput, 2);
  const auto truncatedResult = std::move(arrayPipeline(source)
                                             .filter([&filterCalls](int value) {
                                               ++filterCalls;
                                               return value % 2 == 0;
                                             })
                                             .map([&mapperCalls](int value) {
                                               ++mapperCalls;
                                               return value * 10;
                                             }))
                                   .collectInto(truncatedOutput, 1);

  TEST_ASSERT_EQUAL_UINT(2, completeResult.written);
  TEST_ASSERT_FALSE(completeResult.truncated);
  TEST_ASSERT_EQUAL_INT(20, completeOutput[0]);
  TEST_ASSERT_EQUAL_INT(40, completeOutput[1]);
  TEST_ASSERT_EQUAL_UINT(1, truncatedResult.written);
  TEST_ASSERT_TRUE(truncatedResult.truncated);
  TEST_ASSERT_EQUAL_INT(20, truncatedOutput[0]);
  TEST_ASSERT_EQUAL_INT(77, truncatedOutput[1]);
  TEST_ASSERT_EQUAL_INT(4, filterCalls);
  TEST_ASSERT_EQUAL_INT(2, mapperCalls);
}

void test_array_pipeline_collect_into_static_array_infers_capacity() {
  const int source[] = {1, 2, 3, 4};
  int output[2] = {};

  const auto result = arrayPipeline(source)
                          .filter([](int value) { return value % 2 == 0; })
                          .map([](int value) { return value * 10; })
                          .collectInto(output);

  TEST_ASSERT_EQUAL_UINT(2, result.written);
  TEST_ASSERT_FALSE(result.truncated);
  TEST_ASSERT_EQUAL_INT(20, output[0]);
  TEST_ASSERT_EQUAL_INT(40, output[1]);
}

void test_array_pipeline_collect_into_static_array_reports_truncation() {
  const int source[] = {1, 2, 3};
  int output[1] = {};
  int mapperCalls = 0;

  const auto result = arrayPipeline(source)
                          .map([&mapperCalls](int value) {
                            ++mapperCalls;
                            return value * 10;
                          })
                          .collectInto(output);

  TEST_ASSERT_EQUAL_UINT(1, result.written);
  TEST_ASSERT_TRUE(result.truncated);
  TEST_ASSERT_EQUAL_INT(10, output[0]);
  TEST_ASSERT_EQUAL_INT(3, mapperCalls);
}

void test_array_pipeline_collect_into_accepts_null_output_with_zero_capacity() {
  const int source[] = {1};

  const auto result = std::move(arrayPipeline(source))
                          .collectInto(static_cast<int*>(nullptr), 0);

  TEST_ASSERT_EQUAL_UINT(0, result.written);
  TEST_ASSERT_TRUE(result.truncated);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_array_pipeline_defers_filtering_and_mapping_until_for_each);
  RUN_TEST(test_array_pipeline_filters_before_mapping_and_preserves_order);
  RUN_TEST(test_array_pipeline_empty_source_invokes_no_callbacks);
  RUN_TEST(test_array_pipeline_invokes_callbacks_for_accepted_elements_only);
  RUN_TEST(test_array_pipeline_for_each_without_filter_observes_all_elements_in_order);
  RUN_TEST(test_array_pipeline_pointer_length_processes_selected_subrange_in_order);
  RUN_TEST(test_array_pipeline_mapper_type_change_reaches_for_each);
  RUN_TEST(test_array_pipeline_find_returns_first_transformed_match_and_short_circuits);
  RUN_TEST(test_array_pipeline_find_preserves_output_when_no_element_matches);
  RUN_TEST(test_array_pipeline_any_returns_on_first_match);
  RUN_TEST(test_array_pipeline_all_stops_at_first_non_match_and_empty_is_true);
  RUN_TEST(test_array_pipeline_count_and_reduce_consume_pipeline_output);
  RUN_TEST(test_array_pipeline_collect_into_preserves_order_and_reports_capacity);
  RUN_TEST(test_array_pipeline_collect_into_static_array_infers_capacity);
  RUN_TEST(test_array_pipeline_collect_into_static_array_reports_truncation);
  RUN_TEST(test_array_pipeline_collect_into_accepts_null_output_with_zero_capacity);
  return UNITY_END();
}
