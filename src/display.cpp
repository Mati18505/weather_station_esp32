#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int WIDTH = 16;
const int HEIGHT = 2;
const int SCROLL_DELAY_PER_CHAR_MS = 300;

LiquidCrystal_I2C lcd(0x27, WIDTH, HEIGHT);

struct FirstLineDisplayData {
  int temp;
  int humidity;
};

String format_first_line(int temp, unsigned int humidity) {
  char buffer[17];
    sprintf(buffer, "*C: %d  %%:%d", temp, humidity);

  return String(buffer);
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
  String text = "          " + data.scrollable_text + "         ";
  
  lcd.setCursor(0, 1);
  lcd.print(text.substring(data.pos, data.pos + WIDTH));
  
  data.pos++;
  if (data.pos > text.length() - WIDTH) 
      data.pos = 0;
}

void refresh_display(FirstLineDisplayData data) {
  String first_line = format_first_line(data.temp, data.humidity);
  lcd.print(first_line);
}


void setup() {
 Wire.begin(20,21);
 lcd.init();
 lcd.backlight();


  FirstLineDisplayData data;
  data.temp = 36;
  data.humidity = 67;

  refresh_display(data);
}

unsigned long lastScroll = 0;

void loop() {
  unsigned long now = millis();

  ScrollableTextData scrollable_data = ScrollableTextData::new("light intensity drizzle");

  if (now - lastScroll >= SCROLL_DELAY_PER_CHAR_MS) {
    scroll_system(scrollable_data);
    lastScroll = now;
  }
}