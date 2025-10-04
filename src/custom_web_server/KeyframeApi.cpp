#include "CustomWebServer.h"
#include <ArduinoJson.h>
#include <ArduinoLog.h>
#include <input/TimeInputModule.h>

void CustomWebServer::GetKeyframes(AsyncWebServerRequest* request) {
  JsonDocument json;

  for(const Keyframe& keyframe: TimeInputModule::CurrentKeyframes()) {
    JsonObject outputKeyframe = json.add<JsonObject>();
    outputKeyframe[F("fractionOfSolarDay")] = keyframe.fractionOfSolarDay;
    outputKeyframe[F("colorTemperature")] = keyframe.colorTemperature;
    outputKeyframe[F("brightness")] = keyframe.brightness;
  }

  char output[1024];
  serializeJson(json, output);
  Log.infoln(F("GET /keyframes: %s"), output);

  request->send(200, F("application/json"), output);
}

void CustomWebServer::PostKeyframes(AsyncWebServerRequest* request) {

}
