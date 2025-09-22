#include <Arduino.h>
#include <WiFi.h>

#include "debug/debug.h"
#include "secrets.h"
#include "timekeeping/TimekeepingModule.h"
#include "output/OutputModule.h"

TimekeepingModule timekeeping;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  BlinkingLight::Initialize();
  OutputModule::Initialize();
  timekeeping.begin();

  delay(1000);

  Serial.printf("Connecting to Wi-Fi %s...", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void loop() {
  timekeeping.update();

  auto now = timekeeping.now();
  if (now.isValid()) {
    OutputModule::Update(now);
  }

  delay(250);
}
