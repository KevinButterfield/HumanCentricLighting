#include "CustomWebServer.h"
#include <ArduinoLog.h>
#include <timekeeping/SolarTime.h>
#include <ArduinoJson.h>

void CustomWebServer::SetupCurrentTimeApi(WebServer* server) {
  server->on("/current_time", HTTP_GET, [server]() {
    SolarTime current = SolarTime::Now();
    JsonDocument json;

    json["sunriseHours"] = current.sunriseHours;
    json["sunsetHours"] = current.sunsetHours;
    json["currentFraction"] = current.currentFraction;
    
    char output[256];
    serializeJson(json, output);

    Log.infoln("GET /current_time: %s", output);

    server->sendHeader("Access-Control-Allow-Origin", "*");
    server->send(200, "application/json", output);
  });
}