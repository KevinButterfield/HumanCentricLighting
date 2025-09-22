#include <Arduino.h>
#include <WiFi.h>
#include "secrets.h"

const int LED_PIN = 2;

void setup() {
  Serial.begin(115200);
  BlinkingLight::Initialize();

  delay(1000);

  Serial.printf("Connecting to Wi-Fi %s...", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(WiFi.status());
  }

  Serial.println("\nWi-Fi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  BlinkingLight::Flash();

  digitalWrite(LED_PIN, HIGH);
}

void loop() {
  time_t now = time(nullptr);
  Serial.print("Current time: ");
  Serial.println(ctime(&now));
  delay(2000);
}
