#pragma once
#include <string>
#include <functional>

#include "weather.h"

using Weather = weather::Weather;

class WeatherServer;
using WeatherHandler = std::function<std::string()>;
using SetupWebServer = void(WeatherHandler);

class WeatherServer {
 public:
  struct Impl {
    SetupWebServer* setup;
  } impl;

  WeatherServer(Impl impl) : impl(impl) {
    if (impl.setup) {
      impl.setup([this]() { return weather::serialize_weather(this->weather); });
    }
  }

  Weather weather;
};
