#include <WiFi.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <LittleFS.h> 
#include <common/secrets.h>
#include <optional>

#include <network.h>

const int MAX_WIFI_CONNECT_ATTEMPTS = 10;

String city = "Braniewo";
String url = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "&appid=" + apiKey + "&units=metric";

static WebServer server(80);

bool try_connect_wifi();
std::optional<Weather> fetch_weather();
void weatherHandler(Weather& weather);

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

bool is_wifi_connected() {
  return WiFi.status() == WL_CONNECTED;
}

bool try_connect_wifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);

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

int http_get(const String& url, std::string& outPayload) {
  HTTPClient http;
  http.begin(url);
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
