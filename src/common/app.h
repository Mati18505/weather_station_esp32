#pragma once
#include <cmath>
#include <string_view>
#include <string>
#include <optional>
#include <cstdint>
#include <memory>

#include "config.h"
#include "network.h"
#include "lcd.h"
#include "format.h"

namespace app {
using Weather = weather::Weather;

using Log = void(std::string_view text);
using IsConnected = bool();
using HandleConnections = void();

struct Hardware {
  Log* log_msg;
  IsConnected* is_connected;
  HandleConnections* handle_connections;
  HTTPGet* http_get;
  LCDPrint* lcd_print;
};

struct WifiRetryState {
  int attempts = 0;
  uint64_t next_try_at = 0;
}; 

class Application {
public:
  std::shared_ptr<Weather> weather = std::make_shared<Weather>();

  Application(Hardware hw) : hw(hw) {}

  void loop(uint64_t now) {
    const bool connected = is_connected();
    handle_connections();

    if (!connected) {
      if (now >= wifi_retry_state.next_try_at) {
        wifi_retry_state.attempts++;
        wifi_retry_state.next_try_at = now + 500;
        log("Łączenie z wifi...");
      }
    }

    if (connected) {
      if (now - lastFetch >= WEATHER_FETCH_DELAY_MS) {
        lastFetch = now;

        log("pobieranie danych");
        fetch_and_update_weather();
        refresh_weather_display();
      }
    }

    if (now - lastScroll >= SCROLL_DELAY_PER_CHAR_MS) {
      lastScroll = now;

      if (hw.lcd_print) {
        scroll_system(lcd_second_row, *hw.lcd_print);
      }
    }
  }

private:
  ScrollableTextData lcd_second_row {};
  uint64_t lastFetch = 0;
  uint64_t lastScroll = 0;
  WifiRetryState wifi_retry_state {};
  Hardware hw;

  void update_weather(const Weather& new_weather) {
    if (new_weather.desc != weather->desc) {
      lcd_second_row = ScrollableTextData::create(new_weather.desc);
    }
    *weather = new_weather;
  }

  void fetch_and_update_weather() {
    if (hw.http_get) {
      FetchResult result = fetch_weather(*hw.http_get);

      if (result.weather.has_value()) {
        update_weather(result.weather.value());
      } else if (result.error.has_value()) {
        log(fetch_error_to_string(result.error.value()));
      }
    }
  }

  void log(std::string_view message) {
    if (hw.log_msg) {
      hw.log_msg(message);
    }
  }

  void refresh_weather_display() {
    if (hw.lcd_print) {
      FirstLineDisplayData data {
        .temp = std::round(weather->temperature),
        .humidity = weather->humidity,
      };

      refresh_display(data, *hw.lcd_print);
    }
  }

  void handle_connections() {
    if (hw.handle_connections) {
      hw.handle_connections();
    }
  }

  bool is_connected() {
    return hw.is_connected ? hw.is_connected() : true;
  }
};
}
