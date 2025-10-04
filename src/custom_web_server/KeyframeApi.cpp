#include "CustomWebServer.h"
#include <ArduinoJson.h>
#include <ArduinoLog.h>
#include <input/TimeInputModule.h>

void getKeyframes(WebServer* server) {
  auto keyframes = TimeInputModule::CurrentKeyframes();
  JsonDocument outputKeyframes;

  for(int i = 0; i < KEYFRAME_COUNT; i++) {
    JsonObject keyframe = outputKeyframes.add<JsonObject>();
    keyframe["fractionOfSolarDay"] = keyframes[i].fractionOfSolarDay;
    keyframe["colorTemperature"] = keyframes[i].colorTemperature;
    keyframe["brightness"] = keyframes[i].brightness;
  }

  char output[1024];
  serializeJson(outputKeyframes, output);
  Log.infoln("GET /keyframes: %s", output);

  server->sendHeader("Access-Control-Allow-Origin", "*");
  server->send(200, "application/json", output);
}

void postKeyframes(WebServer* server) {

}

void CustomWebServer::SetupKeyframeApi(WebServer* server) {
  server->on("/keyframes", HTTP_GET, [server]() {
    getKeyframes(server);
  });

  server->on("/keyframes", HTTP_POST, [server]() {
    postKeyframes(server);
  });
}