#include "CustomWebServer.h"
#include <ArduinoLog.h>
#include <timekeeping/SolarTime.h>
#include <ArduinoJson.h>

void CustomWebServer::GetCurrentTime(AsyncWebServerRequest *request) {
  SolarTime current = SolarTime::Now();
  JsonDocument json;

  json[F("sunriseHours")] = current.sunriseHours;
  json[F("sunsetHours")] = current.sunsetHours;
  json[F("currentFraction")] = current.currentFraction;

  char output[256];
  serializeJson(json, output);

  Log.infoln(F("GET /current_time: %s"), output);

  request->send(200, F("application/json"), output);
}