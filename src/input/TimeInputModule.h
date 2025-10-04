#pragma once
#include <output/OutputModule.h>

struct Keyframe {
  float fractionOfSolarDay;
  uint16_t colorTemperature;
  uint8_t brightness;
};

constexpr int KEYFRAME_COUNT = 11;

namespace TimeInputModule {
  void Initialize();
  void SetKeyframes(const Keyframe keyframes[]);

  Keyframe* CurrentKeyframes();
  Keyframe CurrentKeyframe(float fractionOfSolarDay);
};