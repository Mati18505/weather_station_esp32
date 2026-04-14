#include <string>
#include <iostream>
#include <cstdint>
#include <chrono>
#include <thread>
#include <string_view>

#include "common/config.h"
#include "common/lcd.h"
#include "common/app.h"

void log_msg(std::string_view text) {
  std::cout << text << std::endl;
}

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

int http_get(std::string_view url, std::string& outPayload) {
  std::cout << "http_get: " << url << std::endl;

  outPayload = R"({
    "main": { "temp": 21.5, "humidity": 60 },
    "weather": [ { "id": 800, "description": "clear sky" } ]
    })";
  return 200;
}

static WeatherHandler weather_handler {};

void setup_web_server(WeatherHandler wh) {
  weather_handler = wh;
}

bool is_connected() {
  return true;
}

app::Hardware make_desktop_hardware() {
    return {
        .log_msg = log_msg,
        .is_connected = is_connected,
        .handle_connections = nullptr,
        .http_get = http_get,
        .lcd_print = lcd_print,
        .setup_web_srv = setup_web_server,
    };
}

int main() {
  lcd_print(Row::First, "first line");
  lcd_print(Row::Second, "second line");
  // This should change displayed text to "1234nd line     ".
  lcd_print(Row::Second, "1234");
  lcd_print(Row::First, "This text is too long for LCD");

  app::Application application { make_desktop_hardware() };
  
  bool should_exit = false;
  uint64_t lastLogServed = 0;

  while (!should_exit) {
    uint64_t now = millis();

    if (now - lastLogServed >= 3000) {
      lastLogServed = now;

      std::string curr_served = weather_handler();
      log_msg(curr_served);
    }

    application.loop(now);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  return 0;
}
