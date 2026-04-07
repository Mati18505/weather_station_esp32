#include <WiFi.h>
#include <WebServer.h>
#include <LittleFS.h> 
#include <string>

#include "common/weather_server.h"

static WebServer server(80);

inline void setup_web_server(WeatherHandler weather_handler) {
  server.on("/weather", HTTP_GET, [weather_handler]() {
    std::string response = weather_handler();

    if (response.empty()) {
      server.send(500, "application/json", "{}");
    } else {
      server.send(200, "application/json", response.c_str());
    }
  });
  server.serveStatic("/", LittleFS, "/index.html");
  server.begin();
}
