# Weather station ESP32
Weather station that fetches data from the OpenWeather API
Then displays it on an LCD screen and serves it via an HTTP server.

## 🎬 Demo
![Desktop simulation](docs/demo.gif)

## 🚀 Quick Start
You must have [Platform IO](https://platformio.org/install/cli) installed.

### Add OpenWeather API key
Create file `weather_station_esp32/src/common/secrets.h` with structure like this:
```cpp
#pragma once
#include <string_view>

constexpr std::string_view WIFI_SSID = "YOUR_WIFI_SSID";
constexpr std::string_view WIFI_PASS = "YOUR_WIFI_PASS";
constexpr std::string_view API_KEY = "YOUR_OPEN_WEATHER_API_KEY";
```

### Build & Upload
```bash
pio run -e esp32c3 -t upload
```

### Send files from `data` to esp FS
```bash
pio run -e esp32c3 -t uploadfs
```

Open your browser and go to esp IP on port 80.

## Tests
### Run unit tests
```bash
pio test
```

### Run desktop simulation
```bash
pio run -e desktop -t exec
```

## Project structure
`data`: static web frontend (HTML/CSS/JS served from ESP filesystem)
`lib`: pure functions, that are unit tested
`test`: unit testing with [unity testing framework](https://www.throwtheswitch.org/unity)
`src`:
- `common`: platform-independent code shared by desktop and esp
- `desktop`: platform-specific native code
- `esp32`: platform-specific esp code
