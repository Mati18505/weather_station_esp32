#include <unity.h>

void test_basic() {
  TEST_ASSERT_EQUAL_UINT32(wrap_increment(0, 5), 1);
  TEST_ASSERT_EQUAL_UINT32(wrap_increment(4, 5), 5);
  TEST_ASSERT_EQUAL_UINT32(wrap_increment(5, 5), 0);
}

void test_wrap_zero() {
  TEST_ASSERT_EQUAL_UINT32(wrap_increment(0, 0), 0);
}

void test_wrap_increment() {
  RUN_TEST(test_basic);
  RUN_TEST(test_wrap_zero);
}
