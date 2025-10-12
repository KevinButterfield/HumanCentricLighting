#include "CustomWebServer.h"
#include "secrets.h"

#include <Arduino.h>
#include <ArduinoLog.h>
#include <LittleFS.h>
#include <WiFi.h>
#include <ESPmDNS.h>

const char *const TASK_NAME = "WebServerTask";
const int STACK_SIZE_BYTES = 8192;
const int TASK_PRIORITY = 1;

AsyncWebServer server(80);

void CustomWebServer::ConnectToWiFi()
{
  Log.noticeln(F("WiFi Connecting to %s..."), WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void CustomWebServer::StartServer()
{
  if (!LittleFS.begin())
  {
    Log.errorln(F("[Web] Failed to mount LittleFS"));
    vTaskDelete(NULL);
    return;
  }

  while (WiFi.status() != WL_CONNECTED)
  {
    Log.verboseln(F("[Web] Waiting for WiFi connection..."));
    vTaskDelay(pdMS_TO_TICKS(250));
  }

  if (!MDNS.begin("HumanCentricLighting"))
  {
    Log.errorln("[Web] Failed to start MDNS service");
  }

  SetupKeyframesApi(server);
  SetupCurrentTimeApi(server);
  server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");

  DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Origin"), F("*"));
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, POST, DELETE, OPTIONS");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Content-Type");
  server.begin();

  Log.infoln(F("[Web] Server started at http://%s:%d"), WiFi.localIP().toString().c_str(), 80);
}
