#include "CustomWebServer.h"
#include <ArduinoJson.h>
#include <ArduinoLog.h>
#include <input/TimeInputModule.h>

void CustomWebServer::GetKeyframes(AsyncWebServerRequest* request) {
  auto keyframes = TimeInputModule::CurrentKeyframes();
  JsonDocument outputKeyframes;

  for(int i = 0; i < KEYFRAME_COUNT; i++) {
    JsonObject keyframe = outputKeyframes.add<JsonObject>();
    keyframe[F("fractionOfSolarDay")] = keyframes[i].fractionOfSolarDay;
    keyframe[F("colorTemperature")] = keyframes[i].colorTemperature;
    keyframe[F("brightness")] = keyframes[i].brightness;
  }

  char output[1024];
  serializeJson(outputKeyframes, output);
  Log.infoln(F("GET /keyframes: %s"), output);

  request->send(200, F("application/json"), output);
}

void CustomWebServer::PostKeyframes(AsyncWebServerRequest* request) {

}
