#include "common/config.h"
#include "common/lcd.h"
#include <string>
#include <iostream>
#include <cstdint>
#include <chrono>
#include <thread>

void lcd_print(Row row, std::string_view text) {
  static std::string lcd[LCD_HEIGHT] = {
    std::string(LCD_WIDTH, ' '),
    std::string(LCD_WIDTH, ' ')
  };

  int row_index = 0;

  switch (row) {
    case Row::First:  row_index = 0; break;
    case Row::Second: row_index = 1; break;
  }

  for (size_t i = 0; i < text.size() && i < LCD_WIDTH; i++) {
    lcd[row_index][i] = text[i];
  }

  std::cout << "+----------------+\n";
  std::cout << "|" << lcd[0] << "|\n";
  std::cout << "|" << lcd[1] << "|\n";
  std::cout << "+----------------+\n";
  std::cout << std::endl;
}

uint64_t millis() {
    using namespace std::chrono;
    static auto start = steady_clock::now();
    return duration_cast<milliseconds>(steady_clock::now() - start).count();
}

int main() {
  lcd_print(Row::First, "first line");
  lcd_print(Row::Second, "second line");
  // This should change displayed text to "1234nd line     ".
  lcd_print(Row::Second, "1234");
  lcd_print(Row::First, "This text is too long for LCD");
  
  static ScrollableTextData scroll_data = ScrollableTextData::create("scrollable text");

  uint64_t last_scroll = millis();
  bool should_exit = false;

  while (!should_exit) {
    uint64_t now = millis();

    if (now - last_scroll >= SCROLL_DELAY_PER_CHAR_MS) {
      last_scroll = now;
      scroll_system(scroll_data, lcd_print);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  return 0;
}
