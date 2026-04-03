#include <WiFi.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <LittleFS.h> 
#include <optional>
#include <string>

#include <common/config.h>
#include <weather.h>

static WebServer server(80);

bool try_connect_wifi();
void weatherHandler(Weather& weather);

bool is_wifi_connected() {
  return WiFi.status() == WL_CONNECTED;
}

bool try_connect_wifi() {
  WiFi.begin(WIFI_SSID.data(), WIFI_PASS.data());

  retry_until_success([](){
    if (is_wifi_connected()) {
      return true;
    }

    delay(500);
    Serial.print(".");

    return false;
  }, MAX_WIFI_CONNECT_ATTEMPTS);

  return is_wifi_connected();
}

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
  Serial.println("Łączenie z wifi.");

  if (try_connect_wifi()) {
    Serial.println("\nPołączono z wifi!");
  } else {
    Serial.printf("\nNie udało się połączyć z wifi po %d próbach!\n", MAX_WIFI_CONNECT_ATTEMPTS);
    ESP.restart();
  }

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
