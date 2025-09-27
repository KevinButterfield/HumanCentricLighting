#include <Arduino.h>
#include <WiFi.h>

#include "debug/debug.h"
#include "secrets.h"
#include "timekeeping/TimekeepingModule.h"
#include "input/InputModule.h"
#include "output/OutputModule.h"

TimekeepingModule timekeeping;

void setup() {
  Serial.begin(115200);
  BlinkingLight::Initialize();
  InputModule::Initialize();
  OutputModule::Initialize();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  timekeeping.begin();
}

void loop() {
  timekeeping.update();

  OutputModule::Update(InputModule::WarmValue(), InputModule::CoolValue());

  delay(250);
}
