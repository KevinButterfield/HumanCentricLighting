#include "CustomWebServer.h"
#include <ArduinoLog.h>
#include <timekeeping/CurrentSolarTime.h>

void CustomWebServer::SetupCurrentTimeApi(WebServer* server) {
  server->on("/current_time", HTTP_GET, [server]() {
    float currentFraction = CurrentFractionOfSolarDay();
    String formattedFraction = String(currentFraction, 3);

    Log.infoln("GET /current_time: %s", formattedFraction.c_str());

    server->sendHeader("Access-Control-Allow-Origin", "*");
    server->send(200, "application/json", formattedFraction);
  });
}