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

constexpr int KEYFRAME_COUNT = 11;

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