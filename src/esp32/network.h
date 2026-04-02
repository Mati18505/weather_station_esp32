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
std::optional<Weather> fetch_weather();
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

int http_get(const std::string& url, std::string& outPayload) {
  HTTPClient http;
  http.begin(url.c_str());
  int code = http.GET();

  if (code > 0) {
    outPayload = std::string(http.getString().c_str());
  }

  http.end();
  return code;
}

std::optional<Weather> fetch_weather() {
  if (!is_wifi_connected()) {
    Serial.println("brak połączenia wifi");
    return std::nullopt;
  }
  const std::string url = build_weather_url(BASE_URL, CITY, API_KEY);
  std::string payload {};

  int code = http_get(url, payload);
  if (code <= 0) {
    Serial.println("Błąd połączenia http");
    return std::nullopt;
  }
  if (code != 200) {
    Serial.printf("Błąd http: %d\n", code);
    return std::nullopt;
  }

  std::optional<Weather> result = parse_weather_json(payload);

  if (!result.has_value()) {
    Serial.println("Błąd JSON");
  }

  return result;
}

void setup_network(const Weather& weather) {
  Serial.println("Łączenie z wifi.");

  if (try_connect_wifi()) {
    Serial.println("\nPołączono z wifi!");
  } else {
    Serial.printf("\nNie udało się połączyć z wifi po %d próbach!\n", MAX_WIFI_CONNECT_ATTEMPTS);
    ESP.restart();
  }

  server.on("/weather", HTTP_GET, [&weather]() {
    std::string response = serialize_weather(weather);

    if (response.empty()) {
      server.send(500, "application/json", "{}");
    } else {
      server.send(200, "application/json", response.c_str());
    }
  });
  server.serveStatic("/", LittleFS, "/index.html");
  server.begin();
}
