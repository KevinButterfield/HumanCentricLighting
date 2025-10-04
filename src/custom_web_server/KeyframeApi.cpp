#include "CustomWebServer.h"
#include <input/TimeInputModule.h>
#include <ArduinoLog.h>

void CustomWebServer::SetupKeyframeApi(WebServer* server) {
  server->on("/keyframes", HTTP_GET, [server]() {
    auto keyframes = TimeInputModule::CurrentKeyframes();
    String json = "[";
    for(int i = 0; i < KEYFRAME_COUNT; i++) {
      Keyframe keyframe = keyframes[i];
      json += "{\"fractionOfSolarDay\":" + String(keyframe.fractionOfSolarDay) +
              ",\"colorTemperature\":" + String(keyframe.colorTemperature) +
              ",\"brightness\":" + String(keyframe.brightness) + "},";
    }
    if (json.endsWith(",")) {
      json.remove(json.length() - 1);
    }
    json += "]";

    Log.infoln("GET /keyframes: %s", json.c_str());

    server->sendHeader("Access-Control-Allow-Origin", "*");
    server->send(200, "application/json", json);
  });
}