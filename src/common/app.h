#pragma once
#include <cmath>
#include <string_view>
#include <string>
#include <optional>
#include <cstdint>

#include "config.h"
#include "network.h"
#include "lcd.h"
#include "format.h"
#include "weather_server.h"
#include "weather_desc.h"

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
  SetupWebServer* setup_web_srv;
};

struct WifiRetryState {
  int attempts = 0;
  uint64_t next_try_at = 0;
}; 

class Application {
public:
  Weather weather {};
  WeatherServer weather_srv;

  Application(Hardware hw)
    : hw(hw),
      weather_srv(WeatherServer::Impl{
          .setup = hw.setup_web_srv,
      }) {}

  void loop(uint64_t now) {
    const bool connected = is_connected();
    handle_connections();

    if (!connected) {
      if (now >= wifi_retry_state.next_try_at) {
        wifi_retry_state.attempts++;
        wifi_retry_state.next_try_at = now + 500;

        log("Łączenie z wifi...");
        display_status("Laczenie z wifi...");
      }
    }

    if (!was_connected && connected) {
      // connected event
      log("pobieranie danych");
      fetch_and_update_weather();
      refresh_weather_display();

      if (hw.lcd_print) {
        scroll_system(lcd_second_row, *hw.lcd_print);
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

    was_connected = connected;
  }

private:
  ScrollableTextData lcd_second_row {};
  uint64_t lastFetch = 0;
  uint64_t lastScroll = 0;
  WifiRetryState wifi_retry_state {};
  Hardware hw;
  bool was_connected = false;

  void update_weather(const Weather& new_weather) {
    if (new_weather.weather_code != weather.weather_code) {
      weather::WeatherCode weather_code = new_weather.weather_code;
      std::string_view desc = weather::to_string(weather_code);

      lcd_second_row = ScrollableTextData::create(std::string(desc));
    }
    weather = new_weather;
    weather_srv.weather = new_weather;
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
        .temp = static_cast<int>(std::round(weather.temperature)),
        .humidity = weather.humidity,
      };

      refresh_display(data, *hw.lcd_print);
    }
  }

  void display_status(std::string_view status) {
    if (hw.lcd_print) {
      hw.lcd_print(Row::First, status);
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
