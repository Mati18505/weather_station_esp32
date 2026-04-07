#include <unity.h>

#include <display.h>
#include <string>
#include <cstring>

void test_scrollable(const char* text) {
  constexpr size_t leading = 16;
  constexpr size_t trailing = 16;

  const size_t text_len = strlen(text);
  const std::string result = display::format_scrollable(text);
  const size_t start = leading; 
  const size_t end = leading + text_len; 

  TEST_ASSERT_EQUAL_UINT32(leading + text_len + trailing, result.length());

  const std::string leading_str = std::string(leading, ' ');
  const std::string trailing_str = std::string(trailing, ' ');
  
  const std::string leading_sub = result.substr(0, start);
  const std::string middle_sub  = result.substr(start, text_len);
  const std::string trailing_sub = result.substr(end);

  TEST_ASSERT_EQUAL_STRING(leading_str.c_str(), leading_sub.c_str());
  TEST_ASSERT_EQUAL_STRING(text, middle_sub.c_str());
  TEST_ASSERT_EQUAL_STRING(trailing_str.c_str(), trailing_sub.c_str());
}

void test_empty_string() {
  test_scrollable("");
}

void test_simple_text() {
  test_scrollable("abc");
}

void test_single_char() {
  test_scrollable("x");
}

void test_text_with_spaces() {
  test_scrollable("a b c");
}

void test_long_text() {
  test_scrollable("HelloWorld123456");
}

void test_format_scrollable() {
  RUN_TEST(test_empty_string);
  RUN_TEST(test_simple_text);
  RUN_TEST(test_single_char);
  RUN_TEST(test_text_with_spaces);
  RUN_TEST(test_long_text);
}
