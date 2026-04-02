#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <cstddef>
#include <display.h>

const int LCD_WIDTH = 16;
const int LCD_HEIGHT = 2;

LiquidCrystal_I2C lcd(0x27, LCD_WIDTH, LCD_HEIGHT);

struct FirstLineDisplayData {
  int temp;
  int humidity;
};

struct ScrollableTextData {
  String scrollable_text;
  size_t pos = 0;

  static ScrollableTextData create(String text) {
    return ScrollableTextData {
      .scrollable_text = text,
      .pos = 0,
    };
  }
};

void scroll_system(ScrollableTextData& data) {
  std::string text = format_scrollable(data.scrollable_text.c_str());

  lcd.setCursor(0, 1);
  lcd.print(text.substr(data.pos, LCD_WIDTH).c_str());

  data.pos = wrap_increment(data.pos, text.length() - LCD_WIDTH);
}

void refresh_display(FirstLineDisplayData data) {
  std::string first_line = format_first_line(data.temp, data.humidity);
  lcd.setCursor(0, 0);
  lcd.print(first_line.c_str());
}

void setup_lcd() {
  Wire.begin(20,21);
  lcd.init();
  lcd.backlight();
}
