#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WebServer.h>
#include <LittleFS.h> 
#include <secrets.h>
#include <display.h>
#include <optional>

const int SECOND_TO_MS = 1000;
const int WEATHER_FETCH_DELAY_MS = 5 * SECOND_TO_MS;
const int MAX_WIFI_CONNECT_ATTEMPTS = 10;

String city = "Braniewo";
String url = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "&appid=" + apiKey + "&units=metric";

struct Weather {
  float temperature;
  int humidity;
  String desc;
} weather;

WebServer server(80);

bool try_connect_wifi();
std::optional<Weather> fetch_weather();
void weatherHandler(Weather& weather);

void setup() {
  Serial.begin(9600);
  Serial.println("Łączenie z wifi.");

  if(!LittleFS.begin(true)) {
    Serial.println("Nie udało się zamontować systemu plików LittleFS");
  }

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

  setup_lcd();
}

void weatherHandler(Weather& weather) {
  StaticJsonDocument<200> doc;

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

    StaticJsonDocument<1024> doc;
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

unsigned long lastFetch = 0;
unsigned long lastScroll = 0;

void loop() {
  server.handleClient();

  unsigned long now = millis();

  if (now - lastFetch >= WEATHER_FETCH_DELAY_MS) {
    lastFetch = now;

    std::optional<Weather> maybe_weather = fetch_weather();
    if (maybe_weather) {
      weather = *maybe_weather;
    }

    FirstLineDisplayData data {
      .temp = weather.temperature,
      .humidity = weather.humidity,
    };

    refresh_display(data);
  }

  static ScrollableTextData scrollable_data = ScrollableTextData::create(weather.desc);

  if (now - lastScroll >= SCROLL_DELAY_PER_CHAR_MS) {
    lastScroll = now;

    scroll_system(scrollable_data);
  }
}
