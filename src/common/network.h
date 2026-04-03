#pragma once
#include <functional>
#include <string_view>
#include <string>
#include <optional>
#include <variant>

#include "config.h"
#include "weather.h"

using HTTPCode = int;
using HTTPGet = std::function<HTTPCode(std::string_view url, std::string& outPayload)>;

enum class FetchErrorKind {
  HTTPCannotConnect,
  HTTPErrorCode,
  WeatherParseError,
};

struct FetchError {
  FetchErrorKind kind;
  // only HTTPErrorCode
  HTTPCode code;
};

struct FetchResult {
  std::optional<Weather> weather;
  std::optional<FetchError> error;
};

FetchResult fetch_weather(HTTPGet http_get) {
  const std::string url = build_weather_url(BASE_URL, CITY, API_KEY);
  std::string payload {};

  int code = http_get(url, payload);
  if (code <= 0) {
    return FetchResult {
      .weather = std::nullopt,
      .error = FetchError { FetchErrorKind::HTTPCannotConnect, 0 },
    };
  }
  if (code != 200) {
    return FetchResult {
      .weather = std::nullopt,
      .error = FetchError { FetchErrorKind::HTTPErrorCode, code },
    };
  }

  std::optional<Weather> result = parse_weather_json(payload);

  if (!result.has_value()) {
    return FetchResult {
      .weather = std::nullopt,
      .error = FetchError { FetchErrorKind::WeatherParseError, 0 },
    };
  }

  return FetchResult {
    .weather = result,
    .error = std::nullopt,
  };
}
