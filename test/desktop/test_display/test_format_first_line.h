#include <unity.h>

#include <display.h>
#include <string>

void test_first_line(int temp, unsigned int humidity) {
  const std::string result = format_first_line(temp, humidity);
  const std::string expected_start = "*C: " + std::to_string(temp) + "  %:" + std::to_string(humidity);

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

void test_large_values() {
    test_first_line(123, 321);
}

void test_format_first_line() {
    RUN_TEST(test_positive_values);
    RUN_TEST(test_zero_values);
    RUN_TEST(test_negative_temperature);
    RUN_TEST(test_large_values);
}
