#include <unity.h>
#include <cstdint>

#include <weather.h>

void test_returns_true_immediately() {
  uint32_t calls = 0;

  auto condition = [&]() {
    calls++;
    return true;
  };

  bool result = retry_until_success(condition, 5);

  TEST_ASSERT_TRUE(result);
  TEST_ASSERT_EQUAL_UINT32(1, calls);
}

void test_returns_true_after_several_attempts() {
  int calls = 0;

  auto condition = [&]() {
    calls++;
    return calls == 3;
  };

  bool result = retry_until_success(condition, 5);

  TEST_ASSERT_TRUE(result);
  TEST_ASSERT_EQUAL_UINT32(3, calls);
}

void test_returns_false_when_all_attempts_fail() {
  int calls = 0;

  auto condition = [&]() {
    calls++;
    return false;
  };

  bool result = retry_until_success(condition, 4);

  TEST_ASSERT_FALSE(result);
  TEST_ASSERT_EQUAL_UINT32(4, calls);
}

void test_zero_attempts_means_no_calls() {
  int calls = 0;

  auto condition = [&]() {
    calls++;
    return true;
  };

  bool result = retry_until_success(condition, 0);

  TEST_ASSERT_FALSE(result);
  TEST_ASSERT_EQUAL_UINT32(0, calls);
}

void test_retry_until_success() {
  RUN_TEST(test_returns_true_immediately);
  RUN_TEST(test_returns_true_after_several_attempts);
  RUN_TEST(test_returns_false_when_all_attempts_fail);
  RUN_TEST(test_zero_attempts_means_no_calls);
}
