#include <cmath>
#include <string_view>

#include <common/config.h>
#include "lcd.h"
#include "common/network.h"
#include "common/format.h"
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

    if (is_wifi_connected()) {
      Serial.println("pobieranie danych");
      FetchResult result = fetch_weather(http_get);

      if (result.weather.has_value()) {
        update_app_state(app, result.weather.value());

        FirstLineDisplayData data {
          .temp = std::round(app.weather->temperature),
          .humidity = app.weather->humidity,
        };

        refresh_display(data, lcd_print);
      } else if (result.error.has_value()) {
          Serial.println(fetch_error_to_string(result.error.value()).c_str());
        }
      }

    } else {
      Serial.println("brak połączenia wifi");
    }
  }

  if (now - app.lastScroll >= SCROLL_DELAY_PER_CHAR_MS) {
    app.lastScroll = now;

    scroll_system(app.lcd_second_row, lcd_print);
  }
}
