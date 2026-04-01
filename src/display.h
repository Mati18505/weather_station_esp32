#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int LCD_WIDTH = 16;
const int LCD_HEIGHT = 2;
const int SCROLL_DELAY_PER_CHAR_MS = 300;

LiquidCrystal_I2C lcd(0x27, LCD_WIDTH, LCD_HEIGHT);

struct FirstLineDisplayData {
  int temp;
  int humidity;
};

String format_first_line(int temp, unsigned int humidity) {
  char buffer[17];

  int n = snprintf(buffer, sizeof(buffer),
                   "*C: %d  %%:%d", temp, humidity);

  for (int i = n; i < 16; i++) buffer[i] = ' ';
  buffer[16] = '\0';

  return String(buffer);
}

// 10x' ' + text + 9x' '
String format_scrollable(const String& text) {
  String result;
  result.reserve(10 + text.length() + 9);

  for (int i = 0; i < 10; i++) result += ' ';
  result += text;
  for (int i = 0; i < 9; i++) result += ' ';

  return result;
}

struct ScrollableTextData {
  String scrollable_text;
  int pos = 0;

  static ScrollableTextData new(String text) {
    ScrollableTextData data;
    data.pos = 0;
    data.scrollable_text = text;
    return data;
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

void setup() {
  Wire.begin(20,21);
  lcd.init();
  lcd.backlight();
}
