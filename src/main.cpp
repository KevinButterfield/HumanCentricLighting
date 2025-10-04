#include <Arduino.h>
#include <ArduinoLog.h>
#include <WiFi.h>

#include "secrets.h"
#include "custom_debug_utils/BlinkingLight.h"
#include "input/ManualInputModule.h"
#include "input/TimeInputModule.h"
#include "output/OutputModule.h"
#include "timekeeping/TimekeepingModule.h"
#include "custom_web_server/CustomWebServer.h"

TimekeepingModule timekeeping;

void setup() {
  Serial.begin(115200);
  Log.begin(LOG_LEVEL_NOTICE, &Serial, true);
  BlinkingLight::Initialize();
  ManualInputModule::Initialize();
  TimeInputModule::Initialize();
  OutputModule::Initialize();
  timekeeping.begin();

  Log.noticeln(F("WiFi Connecting to %s..."), WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  CustomWebServer::StartServerOnBackgroundCore();
}

void loop() {
  timekeeping.update();

  OutputModule::Update(ManualInputModule::LightValues());

  delay(250);
}
