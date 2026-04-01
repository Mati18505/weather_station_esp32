#include <string>

// 10x' ' + text + 9x' '
static std::string format_scrollable(std::string_view text) {
  std::string result;
  result.reserve(10 + text.length() + 9);

  for (int i = 0; i < 10; i++) result += ' ';
  result += text;
  for (int i = 0; i < 9; i++) result += ' ';

  return result;
}
