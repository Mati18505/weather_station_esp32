#include <unity.h>

#include <display.h>
#include <string>
#include <limits>

void test_first_line(int temp, unsigned int humidity) {
  const std::string result = display::format_first_line(temp, humidity);
  const std::string expected_start = "T: " + std::to_string(temp) + "*  H: " + std::to_string(humidity) + "%";

  TEST_ASSERT_EQUAL_UINT32(16, result.length());

  const std::string leading_sub = result.substr(0, expected_start.length());
  const std::string trailing_sub = result.substr(expected_start.length());

  TEST_ASSERT_EQUAL_STRING(expected_start.c_str(), leading_sub.c_str());
  for (char c : trailing_sub)
  TEST_ASSERT_EQUAL_CHAR(' ', c);
}

void test_positive_values() {
  test_first_line(25, 60);
}

void test_zero_values() {
  test_first_line(0, 0);
}

void test_negative_temperature() {
  test_first_line(-5, 85);
}

void test_max_values_clamp() {
  int temp = std::numeric_limits<int>::max();
  unsigned int humidity = std::numeric_limits<unsigned int>::max();

  const std::string result = display::format_first_line(temp, humidity);
  const std::string expected = "T: 99*  H: 100% ";

  TEST_ASSERT_EQUAL_UINT32(16, result.length());
  TEST_ASSERT_EQUAL_STRING(expected.c_str(), result.c_str());
}

void test_min_values_clamp() {
  int temp = std::numeric_limits<int>::min();
  unsigned int humidity = 0u;

  const std::string result = display::format_first_line(temp, humidity);
  const std::string expected = "T: -99*  H: 0%  ";

  TEST_ASSERT_EQUAL_UINT32(16, result.length());
  TEST_ASSERT_EQUAL_STRING(expected.c_str(), result.c_str());
}

void test_format_first_line() {
  RUN_TEST(test_positive_values);
  RUN_TEST(test_zero_values);
  RUN_TEST(test_negative_temperature);
  RUN_TEST(test_max_values_clamp);
  RUN_TEST(test_min_values_clamp);
}
