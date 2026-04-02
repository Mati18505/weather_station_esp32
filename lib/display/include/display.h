#pragma once
#include <algorithm>
#include <string>

// 10x' ' + text + 9x' '
inline std::string format_scrollable(std::string_view text) {
  std::string result;
  result.reserve(10 + text.length() + 9);

  for (int i = 0; i < 10; i++) result += ' ';
  result += text;
  for (int i = 0; i < 9; i++) result += ' ';

  return result;
}

inline std::string format_first_line(int temp, unsigned int humidity) {
  // Numbers can take max 4 characters, before buffer overflows.
  temp = std::clamp(temp, -999, 9999);
  humidity = std::clamp(humidity, 0u, 9999u);

  char buffer[17];

  int n = snprintf(buffer, sizeof(buffer),
                   "*C: %d  %%:%d", temp, humidity);

  for (int i = n; i < 16; i++) buffer[i] = ' ';
  buffer[16] = '\0';

  return std::string(buffer);
}

inline size_t wrap_increment(size_t x, size_t wrap_at) {
  x++;

  if (x > wrap_at) 
    x = 0;

  return x;
}
