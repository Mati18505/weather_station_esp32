#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const int SECOND_TO_MS = 1000;
const int WEATHER_FETCH_DELAY_MS = 5 * SECOND_TO_MS;
const int MAX_WIFI_CONNECT_ATTEMPTS = 10;

// const char* WIFI_SSID = "Galaxy_A51";
// const char* WIFI_PASS = "18500505";
const char* WIFI_SSID = "Wokwi-GUEST";
const char* WIFI_PASS = "";

String apiKey = "03be7812f2704a2be22abdfd25982343";
String city = "Braniewo";
String url = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "&appid=" + apiKey + "&units=metric";

bool try_connect_wifi();
void check_weather();
void weather_task(void* param);

void setup() {
  Serial.begin(115200);
  Serial.println("Łączenie z wifi.");

  if (try_connect_wifi()) {
    Serial.println("\nPołączono z wifi!");
  } else {
    Serial.printf("\nNie udało się połączyć z wifi po %d próbach!\n", MAX_WIFI_CONNECT_ATTEMPTS);
    ESP.restart();
  }

  xTaskCreate(weather_task, "weather_task", 4096, NULL, 1, NULL);
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

void weather_task(void* param) {
  while (true) {
    if (WiFi.status() == WL_CONNECTED) {
      check_weather();
    }
    vTaskDelay(WEATHER_FETCH_DELAY_MS / portTICK_PERIOD_MS);
  }
}

void check_weather() {
  Serial.print("pobieranie danych");

  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("brak połączenia wifi");
    return;
  }

  HTTPClient http;
  http.begin(url);
  int httpCode = http.GET();

  if (httpCode > 0) {
    String payload = http.getString();

    StaticJsonDocument<1024> doc;
    DeserializationError error = deserializeJson(doc, payload);

    if (!error) {
      float temp = doc["main"]["temp"];
      int humidity = doc["main"]["humidity"];
      const char* desc = doc["weather"][0]["description"];

      Serial.printf("Temperatura: %.1f°C\n", temp);
      Serial.printf("Wilgotność: %d%%\n", humidity);
      Serial.printf("Opis: %s\n", desc);
    } else {
      Serial.println("Błąd JSON");
    }
  } else {
    Serial.println("Błąd połączenia http");
  }

  http.end();
}

void loop() {}