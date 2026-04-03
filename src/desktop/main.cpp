#include "common/config.h"
#include "common/lcd.h"
#include "common/network.h"
#include "common/format.h"
#include <string>
#include <iostream>
#include <cstdint>
#include <chrono>
#include <thread>

struct AppState {
  Weather weather{};
  ScrollableTextData lcd_second_row;
  long lastFetch = 0;
  long lastScroll = 0;
};
static AppState app;

void logger(std::string_view text) {
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
    "weather": [ { "description": "clear sky" } ]
    })";
  return 200;
}

void update_app_state(AppState& app, const Weather& new_weather) {
  if (new_weather.desc != app.weather.desc) {
    app.lcd_second_row = ScrollableTextData::create(new_weather.desc);
  }
  app.weather = new_weather;
}

void loop(uint64_t now) {
  if (now - app.lastFetch >= WEATHER_FETCH_DELAY_MS) {
    app.lastFetch = now;

    logger("pobieranie danych");
    FetchResult result = fetch_weather(http_get);

    if (result.weather.has_value()) {
      update_app_state(app, result.weather.value());

      FirstLineDisplayData data {
        .temp = std::round(app.weather.temperature),
        .humidity = app.weather.humidity,
      };

      refresh_display(data, lcd_print);
    } else if (result.error.has_value()) {
      logger(fetch_error_to_string(result.error.value()));
    }
  }

  if (now - app.lastScroll >= SCROLL_DELAY_PER_CHAR_MS) {
    app.lastScroll = now;

    scroll_system(app.lcd_second_row, lcd_print);
  }
}

int main() {
  lcd_print(Row::First, "first line");
  lcd_print(Row::Second, "second line");
  // This should change displayed text to "1234nd line     ".
  lcd_print(Row::Second, "1234");
  lcd_print(Row::First, "This text is too long for LCD");
  
  bool should_exit = false;

  while (!should_exit) {
    uint64_t now = millis();

    loop(now);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  return 0;
}
