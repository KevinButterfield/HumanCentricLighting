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

void CustomWebServer::PostKeyframes(AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t index, size_t total) {
  JsonDocument json;

  auto error = deserializeJson(json, data, len);
  if (error || !json.is<JsonArray>() || json.as<JsonArray>().size() != 11) {
    request->send(422, F("application/text"), error ? error.c_str() : "Not an array");
    return;
  }

  JsonArray keyframes = json.as<JsonArray>();
  Log.infoln(F("POST /keyframes: %s"), data);

  std::vector<Keyframe> output(keyframes.size());
  for (int i = 0; i < output.size(); ++i) {
    output[i].fractionOfSolarDay = keyframes[i]["fractionOfSolarDay"] | 0.0f;
    output[i].colorTemperature = keyframes[i]["colorTemperature"] | 2700;
    output[i].brightness = keyframes[i]["brightness"] | 100;
  }

  TimeInputModule::SetKeyframes(output);

  request->send(201);
}
