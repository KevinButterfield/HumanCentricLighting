#pragma once
#include <vector>
#include <ArduinoJson.h>
#include <output/OutputModule.h>

struct Keyframe
{
  float fractionOfSolarDay;
  uint16_t colorTemperature;
  uint8_t brightness;
};

constexpr int KEYFRAME_COUNT = 11;

namespace TimeInputModule
{
  void Initialize();

  void SetKeyframes(const JsonArray keyframes, const String &raw);
  std::vector<Keyframe> CurrentKeyframes();

  Keyframe* CurrentKeyframesInternal();
  Keyframe CurrentKeyframe(float fractionOfSolarDay);
};