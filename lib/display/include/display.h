#pragma once
#include <algorithm>
#include <string>

namespace display {
// 16x' ' + text + 16x' '
inline std::string format_scrollable(std::string_view text) {
  std::string result;
  result.reserve(16 + text.length() + 16);

  for (int i = 0; i < 16; i++) result += ' ';
  result += text;
  for (int i = 0; i < 16; i++) result += ' ';

  return result;
}

inline std::string format_first_line(int temp, unsigned int humidity) {
  temp = std::clamp(temp, -99, 99);
  humidity = std::clamp(humidity, 0u, 100u);

  char buffer[17];

  int n = snprintf(buffer, sizeof(buffer),
                   "T: %d*  H: %d%%", temp, humidity);

  int text_end = std::clamp(n, 0, 16);

  for (int i = text_end; i < 16; i++) buffer[i] = ' ';
  buffer[16] = '\0';

  return std::string(buffer);
}

inline size_t wrap_increment(size_t x, size_t wrap_at) {
  x++;

  if (x > wrap_at) 
    x = 0;

  return x;
}
}
