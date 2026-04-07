#pragma once
#include <string>
#include "network.h"

std::string fetch_error_to_string(const FetchError& err) {
  switch (err.kind) {
    case FetchErrorKind::HTTPCannotConnect:
      return "Cannot connect";
    case FetchErrorKind::HTTPErrorCode:
      return "HTTP error " + std::to_string(err.code);
    case FetchErrorKind::WeatherParseError:
      return "Weather parse error";
  }
  return "Unknown error";
}
