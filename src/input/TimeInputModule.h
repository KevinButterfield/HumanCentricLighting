#pragma once
#include <vector>
#include <output/OutputModule.h>

struct Keyframe {
  float fractionOfSolarDay;
  uint16_t colorTemperature;
  uint8_t brightness;
};

constexpr int KEYFRAME_COUNT = 11;

namespace TimeInputModule {
  void Initialize();
  
  void SetKeyframes(const std::vector<Keyframe>& keyframes);
  std::vector<Keyframe> CurrentKeyframes();

  Keyframe CurrentKeyframe(float fractionOfSolarDay);
};