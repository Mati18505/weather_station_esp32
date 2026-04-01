#include <cstring>
#include <unity.h>

#include <display.h>
#include <string>

void test_text(const char* text) {
  const size_t text_len = strlen(text);
  const std::string result = format_scrollable(text);
  const size_t start = 10; 
  const size_t end = 10 + text_len; 

  TEST_ASSERT_EQUAL_UINT32(10 + text_len + 9, result.length());

  const std::string leading = std::string(10, ' ');
  const std::string trailing = std::string(9, ' ');
  
  const std::string leading_sub = result.substr(0, start);
  const std::string middle_sub  = result.substr(start, text_len);
  const std::string trailing_sub = result.substr(end);

  TEST_ASSERT_EQUAL_STRING(leading.c_str(), leading_sub.c_str());
  TEST_ASSERT_EQUAL_STRING(text, middle_sub.c_str());
  TEST_ASSERT_EQUAL_STRING(trailing.c_str(), trailing_sub.c_str());
}

void test_empty_string() {
  test_text("");
}

void test_simple_text() {
  test_text("abc");
}

void test_single_char() {
  test_text("x");
}

void test_text_with_spaces() {
  test_text("a b c");
}

void test_long_text() {
  test_text("HelloWorld123456");
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_empty_string);
  RUN_TEST(test_simple_text);
  RUN_TEST(test_single_char);
  RUN_TEST(test_text_with_spaces);
  RUN_TEST(test_long_text);
  return UNITY_END();
}
