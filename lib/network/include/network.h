#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <string_view>
#include <ArduinoJson.h>

struct Weather {
  float temperature;
  int humidity;
  std::string desc;
};

inline bool retry_until_success(std::function<bool()> condition, uint32_t max_attempts) {
  bool success = false;

  for (uint32_t attempt = 0; attempt < max_attempts; attempt++) {
    if (condition()) {
      success = true;
      break;
    }
  }

  return success;
}

std::optional<Weather> parse_weather_json(std::string_view payload) {
  JsonDocument doc;
  if (deserializeJson(doc, payload)) {
      return std::nullopt;
  }

  float temperature = doc["main"]["temp"];
  int humidity = doc["main"]["humidity"];
  const char* desc = doc["weather"][0]["description"];

  return Weather{
    .temperature = temperature,
    .humidity = humidity,
    .desc = desc,
  };
}
