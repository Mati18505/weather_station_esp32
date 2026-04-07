#pragma once
#include <cmath>
#include <functional>
#include <string_view>
#include <string>
#include <optional>
#include <cstdint>
#include <memory>

#include "config.h"
#include "network.h"
#include "lcd.h"
#include "display.h"
#include "format.h"

namespace app {
using Log = std::function<void(std::string_view text)>;
using IsConnected = std::function<bool()>;
using HandleConnections = std::function<void()>;

struct Hardware {
  Log log_msg;
  IsConnected is_connected;
  HandleConnections handle_connections;
  HTTPGet http_get;
  LCDPrint lcd_print;
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
    const bool connected = hw.is_connected();
    hw.handle_connections();

    if (!connected) {
      if (now >= wifi_retry_state.next_try_at) {
        wifi_retry_state.attempts++;
        wifi_retry_state.next_try_at = now + 500;
        hw.log_msg("Łączenie z wifi...");
      }
    }

    if (connected) {
      if (now - lastFetch >= WEATHER_FETCH_DELAY_MS) {
        lastFetch = now;

        hw.log_msg("pobieranie danych");
        FetchResult result = fetch_weather(hw.http_get);

        if (result.weather.has_value()) {
          update_weather(result.weather.value());

          FirstLineDisplayData data {
            .temp = std::round(weather->temperature),
            .humidity = weather->humidity,
          };

          refresh_display(data, hw.lcd_print);
        } else if (result.error.has_value()) {
          hw.log_msg(fetch_error_to_string(result.error.value()));
        }
      }
    }

    if (now - lastScroll >= SCROLL_DELAY_PER_CHAR_MS) {
      lastScroll = now;

      scroll_system(lcd_second_row, hw.lcd_print);
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
};
}
