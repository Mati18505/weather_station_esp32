#include <display.h>
#include <network.h>
#include <optional>

static Weather weather;

void setup() {
  Serial.begin(9600);

  if(!LittleFS.begin(true)) {
    Serial.println("Nie udało się zamontować systemu plików LittleFS");
  }

  setup_network(weather);
  setup_lcd();
}

unsigned long lastFetch = 0;
unsigned long lastScroll = 0;

void loop() {
  server.handleClient();

  unsigned long now = millis();

  if (now - lastFetch >= WEATHER_FETCH_DELAY_MS) {
    lastFetch = now;

    std::optional<Weather> maybe_weather = fetch_weather();
    if (maybe_weather) {
      weather = *maybe_weather;
    }

    FirstLineDisplayData data {
      .temp = weather.temperature,
      .humidity = weather.humidity,
    };

    refresh_display(data);
  }

  static ScrollableTextData scrollable_data = ScrollableTextData::create(weather.desc);

  if (now - lastScroll >= SCROLL_DELAY_PER_CHAR_MS) {
    lastScroll = now;

    scroll_system(scrollable_data);
  }
}
