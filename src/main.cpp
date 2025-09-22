#include <Arduino.h>
#include <WiFi.h>
#include "secrets.h"
#include "timekeeping/TimekeepingModule.h"

const int LED_PIN = 2;

TimekeepingModule timekeeping;

void setup() {
  Serial.begin(115200);
  BlinkingLight::Initialize();

  delay(1000);

  Serial.printf("Connecting to Wi-Fi %s...", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void loop() {
  timekeeping.update();

  delay(1000);
}
