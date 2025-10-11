#include <Arduino.h>
#include <ArduinoLog.h>
#include <WiFi.h>

#include "custom_debug_utils/BlinkingLight.h"
#include "input/ManualInputModule.h"
#include "input/TimeInputModule.h"
#include "output/OutputModule.h"
#include "timekeeping/TimekeepingModule.h"
#include "custom_web_server/CustomWebServer.h"

TimekeepingModule timekeeping;
constexpr int STACK_SIZE_BYTES = 8192;

void ServerTask(void *parameter)
{
  CustomWebServer::ConnectToWiFi();
  CustomWebServer::StartServer();

  while (true)
  {
    timekeeping.update();
    vTaskDelay(pdMS_TO_TICKS(250));
  }
}

void setup()
{
  Serial.begin(115200);
  Log.begin(LOG_LEVEL_VERBOSE, &Serial, true);
  ManualInputModule::Initialize();
  TimeInputModule::Initialize();
  OutputModule::Initialize();
  timekeeping.begin();

  xTaskCreatePinnedToCore(
      ServerTask,
      "ServerTask",
      STACK_SIZE_BYTES,
      nullptr,
      1,
      nullptr,
      0);
}

void loop()
{
  OutputModule::Update(ManualInputModule::LightValues());
  delay(250);
}