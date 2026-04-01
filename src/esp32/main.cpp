#include <cmath>
#include "display.h"
#include "network.h"
#include <optional>

static const int SECOND_TO_MS = 1000;
static const int WEATHER_FETCH_DELAY_MS = 5 * SECOND_TO_MS;
static const int SCROLL_DELAY_PER_CHAR_MS = 300;

struct AppState {
  Weather weather;
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

void loop() {
  server.handleClient();

  unsigned long now = millis();

  if (now - app.lastFetch >= WEATHER_FETCH_DELAY_MS) {
    app.lastFetch = now;

    std::optional<Weather> maybe_weather = fetch_weather();
    if (maybe_weather.has_value()) {
      if (maybe_weather->desc != app.weather.desc) {
        app.lcd_second_row = ScrollableTextData::create(maybe_weather->desc);
      }
      app.weather = *maybe_weather;
    }

    FirstLineDisplayData data {
      .temp = std::round(app.weather.temperature),
      .humidity = app.weather.humidity,
    };

    refresh_display(data);
  }

  if (now - app.lastScroll >= SCROLL_DELAY_PER_CHAR_MS) {
    app.lastScroll = now;

    scroll_system(app.lcd_second_row);
  }
}
