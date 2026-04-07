#include <string_view>

#include <common/config.h>
#include "lcd.h"
#include "network.h"
#include "common/app.h"

std::optional<app::Application> application;

void log_msg(std::string_view msg) {
  Serial.write(msg.data(), msg.size());
  Serial.flush();
}

bool is_connected() {
  return WiFi.status() == WL_CONNECTED;
}

void handle_connections() {
  server.handleClient();
}

app::Hardware make_esp_hardware() {
    return {
        .log_msg = log_msg,
        .is_connected = is_connected,
        .handle_connections = handle_connections,
        .http_get = http_get,
        .lcd_print = lcd_print,
    };
}

void setup() {
  Serial.begin(9600);

  if(!LittleFS.begin(true)) {
    Serial.println("Nie udało się zamontować systemu plików LittleFS");
  }

  WiFi.begin(WIFI_SSID.data(), WIFI_PASS.data());

  setup_lcd();

  application = app::Application { make_esp_hardware() };
  setup_network(application.value().weather);
}

void loop() {
  unsigned long now = millis();

  application.value().loop(now);
}
