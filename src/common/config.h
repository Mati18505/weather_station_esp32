#pragma once
#include <string_view>
#include "secrets.h"

constexpr int LCD_WIDTH = 16;
constexpr int LCD_HEIGHT = 2;

constexpr int MAX_WIFI_CONNECT_ATTEMPTS = 10;

constexpr std::string_view CITY = "Braniewo";
constexpr std::string_view BASE_URL = "http://api.openweathermap.org/data/2.5/weather?q=";

constexpr int SECOND_TO_MS = 1000;
constexpr int WEATHER_FETCH_DELAY_MS = 5 * SECOND_TO_MS;
constexpr int SCROLL_DELAY_PER_CHAR_MS = 300;
