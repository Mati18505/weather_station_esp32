#include <WiFi.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <LittleFS.h> 
#include <optional>
#include <string>

#include <common/config.h>
#include <weather.h>

static WebServer server(80);

int http_get(std::string_view url, std::string& outPayload) {
  const std::string url_nt(url);

  HTTPClient http;
  http.begin(url_nt.c_str());
  int code = http.GET();

  if (code > 0) {
    outPayload = std::string(http.getString().c_str());
  }

  http.end();
  return code;
}

void setup_network(std::shared_ptr<Weather> weather) {
  server.on("/weather", HTTP_GET, [weather]() {
    std::string response = serialize_weather(*weather);

    if (response.empty()) {
      server.send(500, "application/json", "{}");
    } else {
      server.send(200, "application/json", response.c_str());
    }
  });
  server.serveStatic("/", LittleFS, "/index.html");
  server.begin();
}
