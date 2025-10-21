#pragma once
#include <vector>
#include <ArduinoJson.h>
#include <output/OutputModule.h>

struct Keyframe
{
  float fractionOfSolarDay;
  float brightness;
  int colorTemperature;
};

const float KEYFRAMES[] = { -0.1, 0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1 };
constexpr int KEYFRAME_COUNT = sizeof(KEYFRAMES) / sizeof(KEYFRAMES[0]);

namespace TimeInputModule
{
  void Initialize();
  Light LightValues();

  String ValidateKeyframe(const JsonObject keyframe);
  void SetKeyframes(const JsonArray keyframes, const String &raw);
  void ResetKeyframes();
  
  std::vector<Keyframe> CurrentKeyframes();
  Keyframe* CurrentKeyframesInternal();
};