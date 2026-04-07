#pragma once
#include <cstddef>
#include <functional>
#include <iostream>
#include <string_view>
#include <string>

#include "config.h"
#include "display.h"

enum class Row {
  First,
  Second,
};

using LCDPrint = std::function<void(Row, std::string_view)>;

struct FirstLineDisplayData {
  int temp;
  int humidity;
};

struct ScrollableTextData {
  std::string scrollable_text;
  size_t pos = 0;

  static ScrollableTextData create(std::string text) {
    return ScrollableTextData {
      .scrollable_text = text,
      .pos = 0,
    };
  }
};

void scroll_system(ScrollableTextData& data, LCDPrint print) {
  std::string text = format_scrollable(data.scrollable_text.c_str());
  std::string display_text = text.substr(data.pos, LCD_WIDTH);

  print(Row::Second, display_text);

  data.pos = wrap_increment(data.pos, text.length() - LCD_WIDTH);
}

void refresh_display(FirstLineDisplayData data, LCDPrint print) {
  std::string first_line = format_first_line(data.temp, data.humidity);

  print(Row::First, first_line);
}
