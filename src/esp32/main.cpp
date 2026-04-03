#include <cmath>

#include <common/config.h>
#include "lcd.h"
#include "network.h"

struct AppState {
  std::shared_ptr<Weather> weather = std::make_shared<Weather>();
  ScrollableTextData lcd_second_row;
  long lastFetch = 0;
  long lastScroll = 0;
};
static AppState app;

void setup() {
  Serial.begin(9600);

  if(!LittleFS.begin(true)) {
    Serial.println("Nie udało się zamontować systemu plików LittleFS");
  }

  setup_network(app.weather);
  setup_lcd();
}

void update_app_state(AppState& app, const Weather& new_weather) {
    if (new_weather.desc != app.weather->desc) {
        app.lcd_second_row = ScrollableTextData::create(new_weather.desc);
    }
    *(app.weather) = new_weather;
}

void loop() {
  server.handleClient();

  unsigned long now = millis();

  if (now - app.lastFetch >= WEATHER_FETCH_DELAY_MS) {
    app.lastFetch = now;

    Serial.println("pobieranie danych");

    std::optional<Weather> maybe_weather = fetch_weather();
    if (maybe_weather.has_value()) {
      update_app_state(app, maybe_weather.value());
    }

    FirstLineDisplayData data {
      .temp = std::round(app.weather->temperature),
      .humidity = app.weather->humidity,
    };

    refresh_display(data, lcd_print);
  }

  if (now - app.lastScroll >= SCROLL_DELAY_PER_CHAR_MS) {
    app.lastScroll = now;

    scroll_system(app.lcd_second_row, lcd_print);
  }
}
