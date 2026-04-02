#pragma once

#include <cmath>
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

  JsonObject main = doc["main"].as<JsonObject>();
  if (!main) {
    return std::nullopt;
  }

  JsonArray weather = doc["weather"].as<JsonArray>();
  if (!weather || weather.size() == 0) {
    return std::nullopt;
  }

  float temperature = main["temp"] | NAN;
  int humidity = main["humidity"] | -1;
  std::string desc = weather[0]["description"] | "";

  if (std::isnan(temperature) || humidity < 0) {
      return std::nullopt;
  }

  return Weather{
    .temperature = temperature,
    .humidity = humidity,
    .desc = desc,
  };
}

// Returns serialized weather in json.
// If error occured, returns empty string.
std::string serialize_weather(const Weather& weather) {
  JsonDocument doc;

  doc["temperature"] = weather.temperature;
  doc["humidity"] = weather.humidity;
  doc["desc"] = weather.desc;

  if (doc.overflowed()) {
    return {};
  }

  std::string jsonStr;
  size_t written = serializeJson(doc, jsonStr);

  if (written == 0) {
    return {};
  }

  return jsonStr;
}

std::string build_weather_url(std::string_view base_url, std::string_view city, std::string_view api_key) {
  std::string url;
  url.reserve(128);

  url += base_url;
  url += city;
  url += "&appid=";
  url += api_key;
  url += "&units=metric";

  return url;
}
