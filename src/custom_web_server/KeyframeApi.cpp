#include "CustomWebServer.h"
#include <ArduinoJson.h>
#include <ArduinoLog.h>
#include <input/TimeInputModule.h>

void sendSerializedJson(AsyncWebServerRequest *request)
{
  JsonDocument json;

  for (const Keyframe &keyframe : TimeInputModule::CurrentKeyframes())
  {
    JsonObject outputKeyframe = json.add<JsonObject>();
    outputKeyframe[F("fractionOfSolarDay")] = keyframe.fractionOfSolarDay;
    outputKeyframe[F("colorTemperature")] = keyframe.colorTemperature;
    outputKeyframe[F("brightness")] = keyframe.brightness;
  }

  char output[1024];
  serializeJson(json, output);

  request->send(200, F("application/json"), output);
}

void getKeyframes(AsyncWebServerRequest *request)
{
  Log.infoln(F("GET /keyframes"));

  sendSerializedJson(request);
}

void postKeyframes(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
  JsonDocument json;
  JsonArray keyframes;
  String body(data, len);

  auto error = deserializeJson(json, body);
  keyframes = json.as<JsonArray>();

  if (error || !json.is<JsonArray>() || keyframes.size() != 11)
  {
    request->send(422, F("text/plain"), error ? error.c_str() : "Not an array");
    return;
  }
  else
    for (int i = 0; i < 11; ++i)
    {
      auto validationError = TimeInputModule::ValidateKeyframe(keyframes[i]);

      if (!validationError.isEmpty())
      {
        request->send(422, F("text/plain"), validationError.c_str());
        return;
      }
    }

  Log.infoln(F("POST /keyframes: %s"), body.c_str());

  TimeInputModule::SetKeyframes(keyframes, body);

  sendSerializedJson(request);
}

void resetKeyframes(AsyncWebServerRequest *request)
{
  TimeInputModule::ResetKeyframes();
  
  Log.infoln(F("DELETE /keyframes"));

  sendSerializedJson(request);
}

void CustomWebServer::SetupKeyframesApi(AsyncWebServer &server)
{
  server.on("/keyframes", HTTP_GET, getKeyframes);
  server.on("/keyframes", HTTP_POST, [](AsyncWebServerRequest*) {}, nullptr, postKeyframes);
  server.on("/keyframes", HTTP_DELETE, resetKeyframes);
  server.on("/keyframes", HTTP_OPTIONS, [](AsyncWebServerRequest* request){ request->send(200); });
}