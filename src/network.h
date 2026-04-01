#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WebServer.h>
#include <LittleFS.h> 
#include <secrets.h>
#include <optional>

const int MAX_WIFI_CONNECT_ATTEMPTS = 10;

String city = "Braniewo";
String url = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "&appid=" + apiKey + "&units=metric";

struct Weather {
  float temperature;
  int humidity;
  String desc;
};

static WebServer server(80);

bool try_connect_wifi();
std::optional<Weather> fetch_weather();
void weatherHandler(Weather& weather);

void weatherHandler(Weather& weather) {
  JsonDocument doc;

  doc["temperature"] = weather.temperature;
  doc["humidity"] = weather.humidity;
  doc["desc"] = weather.desc;

  if (doc.overflowed()) {
    Serial.println("JSON overflow!");
    server.send(500, "application/json", "{}");
    return;
  }

  String jsonStr;
  size_t written = serializeJson(doc, jsonStr);

  if (written == 0) {
    Serial.println("Serialization failed");
    server.send(500, "application/json", "{}");
    return;
  }

  server.send(200, "application/json", jsonStr);
}

bool try_connect_wifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  int attempt = 0;

  while (WiFi.status() != WL_CONNECTED && attempt < MAX_WIFI_CONNECT_ATTEMPTS) {
    delay(500);
    Serial.print(".");

    attempt++;
  }

  return WiFi.status() == WL_CONNECTED;
}

std::optional<Weather> fetch_weather() {
  std::optional<Weather> result = std::nullopt;

  Serial.print("pobieranie danych");

  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("brak połączenia wifi");
    return result;
  }

  HTTPClient http;
  http.begin(url);
  int httpCode = http.GET();

  if (httpCode > 0) {
    String payload = http.getString();

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, payload);

    if (!error) {
      float temperature = doc["main"]["temp"];
      int humidity = doc["main"]["humidity"];
      const char* desc = doc["weather"][0]["description"];
      
      Weather weather;

      weather.temperature = temperature;
      weather.humidity = humidity;
      weather.desc = String(desc);

      result = weather;

      Serial.printf("Temperatura: %.1f°C\n", temperature);
      Serial.printf("Wilgotność: %d%%\n", humidity);
      Serial.printf("Opis: %s\n", desc);
    } else {
      Serial.println("Błąd JSON");
    }
  } else {
    Serial.println("Błąd połączenia http");
  }

  Serial.println();

  http.end();

  return result;
}

void setup_network(Weather& weather) {
  Serial.println("Łączenie z wifi.");

  if (try_connect_wifi()) {
    Serial.println("\nPołączono z wifi!");
  } else {
    Serial.printf("\nNie udało się połączyć z wifi po %d próbach!\n", MAX_WIFI_CONNECT_ATTEMPTS);
    ESP.restart();
  }

  server.on("/weather", HTTP_GET, [&weather]() {
    weatherHandler(weather);
  });
  server.serveStatic("/", LittleFS, "/index.html");
  server.begin();
}
