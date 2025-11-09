#include <Arduino.h>
#include <ArduinoLog.h>
#include <WiFi.h>

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

  while(!timekeeping.haveTime()) {
    timekeeping.update();
    vTaskDelay(pdMS_TO_TICKS(500));
  }

  vTaskDelay(portMAX_DELAY);
}

void setup()
{
  Serial.begin(115200);
  Log.begin(LOG_LEVEL_VERBOSE, &Serial, true);
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
  OutputModule::Update(TimeInputModule::LightValues());
  delay(1000);
}