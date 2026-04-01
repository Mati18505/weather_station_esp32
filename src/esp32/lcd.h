#include <Wire.h>
#include <LiquidCrystal_I2C.h>
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
  int pos = 0;

  static ScrollableTextData create(String text) {
    return ScrollableTextData {
      .scrollable_text = text,
      .pos = 0,
    };
  }
};

void scroll_system(ScrollableTextData& data) {
  String text = format_scrollable(data.scrollable_text);

  lcd.setCursor(0, 1);
  lcd.print(text.substring(data.pos, data.pos + LCD_WIDTH));

  data.pos++;
  if (data.pos > text.length() - LCD_WIDTH) 
    data.pos = 0;
}

void refresh_display(FirstLineDisplayData data) {
  String first_line = format_first_line(data.temp, data.humidity);
  lcd.setCursor(0, 0);
  lcd.print(first_line);
}

void setup_lcd() {
  Wire.begin(20,21);
  lcd.init();
  lcd.backlight();
}
