#include "CustomWebServer.h"
#include <ArduinoLog.h>
#include <timekeeping/SolarTime.h>

void CustomWebServer::SetupCurrentTimeApi(WebServer* server) {
  server->on("/current_time", HTTP_GET, [server]() {
    SolarTime current = SolarTime::Now();

    String json = "{\"sunriseHours\":" + String(current.sunriseHours, 3) +
            ",\"sunsetHours\":" + String(current.sunsetHours, 3) +
            ",\"currentFraction\":" + String(current.currentFraction, 3) + "}";

    Log.infoln("GET /current_time: %s", json.c_str());

    server->sendHeader("Access-Control-Allow-Origin", "*");
    server->send(200, "application/json", json);
  });
}