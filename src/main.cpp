#include <Arduino.h>
#include <WiFi.h>

#include "debug/debug.h"
#include "secrets.h"
#include "timekeeping/TimekeepingModule.h"

TimekeepingModule timekeeping;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  BlinkingLight::Initialize();
  timekeeping.begin();

  delay(1000);

  Serial.printf("Connecting to Wi-Fi %s...", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void loop() {
  timekeeping.update();
  delay(1000);
}
