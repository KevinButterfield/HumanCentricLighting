#include "CustomWebServer.h"

#include <Arduino.h>
#include <ArduinoLog.h>
// #include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <WiFi.h>

const char* const TASK_NAME = "WebServerTask";
const int STACK_SIZE_BYTES = 8192;
const int TASK_PRIORITY = 1;

TaskHandle_t serverTaskHandle = nullptr;
WebServer server;

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

  CustomWebServer::SetupKeyframeApi(&server);
  CustomWebServer::SetupCurrentTimeApi(&server);

  server.serveStatic("/", LittleFS, "/");
  server.begin();
  Log.infoln(F("[Web] Server \"started\" at http://%s:%d"), WiFi.localIP().toString().c_str(), 80);

  while (true) {
    server.handleClient();
    vTaskDelay(pdMS_TO_TICKS(10));
  }
  // vTaskDelay(portMAX_DELAY);
}
