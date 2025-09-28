#include <Arduino.h>
#include <ArduinoLog.h>
#include <WiFi.h>

#include "debug/debug.h"
#include "input/InputModule.h"
#include "output/OutputModule.h"
#include "secrets.h"
#include "timekeeping/TimekeepingModule.h"

TimekeepingModule timekeeping;

void setup() {
  Serial.begin(115200);
  Log.begin(LOG_LEVEL_VERBOSE, &Serial, true);
  BlinkingLight::Initialize();
  InputModule::Initialize();
  OutputModule::Initialize();
  timekeeping.begin();

  Log.noticeln(F("WiFi Connecting to %s..."), WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void loop() {
  timekeeping.update();

  OutputModule::Update(InputModule::LightValues());

  delay(250);
}
