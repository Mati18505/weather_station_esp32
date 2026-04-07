#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include <common/config.h>
#include "common/lcd.h"

LiquidCrystal_I2C lcd(0x27, LCD_WIDTH, LCD_HEIGHT);

void lcd_print(Row row, std::string_view text) {
  int row_index = 0;

  switch (row) {
      case Row::First:  row_index = 0; break;
      case Row::Second: row_index = 1; break;
  }

  const std::string null_terminated(text);

  lcd.setCursor(0, row_index);
  lcd.print(null_terminated.c_str());
}

void setup_lcd() {
  Wire.begin(20,21);
  lcd.init();
  lcd.backlight();
}
