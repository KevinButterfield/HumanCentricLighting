#include "CustomWebServer.h"

#include <Arduino.h>
#include <ArduinoLog.h>
#include <LittleFS.h>
#include <WiFi.h>

const char* const TASK_NAME = "WebServerTask";
const int STACK_SIZE_BYTES = 8192;
const int TASK_PRIORITY = 1;

TaskHandle_t serverTaskHandle = nullptr;
AsyncWebServer server(80);

void CustomWebServer::StartServerOnBackgroundCore() {
  if (serverTaskHandle != nullptr) {
    Log.warningln(F("[Web] Server task already running."));
    return;
  }

  xTaskCreatePinnedToCore(
    CustomWebServer::ServerTask,
    TASK_NAME,
    STACK_SIZE_BYTES,
    nullptr,
    TASK_PRIORITY,
    &serverTaskHandle,
    0
  );
}

void CustomWebServer::ServerTask(void* parameter) {
  Log.infoln(F("[Web] Server task started on core %d"), xPortGetCoreID());
  
  if (!LittleFS.begin()) {
    Log.errorln(F("[Web] Failed to mount LittleFS"));
    vTaskDelete(NULL);
    return;
  }

  while(WiFi.status() != WL_CONNECTED) {
    Log.verboseln(F("[Web] Waiting for WiFi connection..."));
    vTaskDelay(pdMS_TO_TICKS(250));
  }

  server.on("/keyframes", HTTP_GET, CustomWebServer::GetKeyframes);
  server.on("/keyframes", HTTP_POST, CustomWebServer::PostKeyframes);
  server.on("/current-time", HTTP_GET, CustomWebServer::GetCurrentTime);
  server.serveStatic("/", LittleFS, "/");

  DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Origin"), F("*"));
  server.begin();

  Log.infoln(F("[Web] Server started at http://%s:%d"), WiFi.localIP().toString().c_str(), 80);

  vTaskDelay(portMAX_DELAY);
}
