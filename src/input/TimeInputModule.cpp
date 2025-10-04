#include "TimeInputModule.h"

#include <ArduinoLog.h>
#include <Preferences.h>
#include <math.h>

constexpr int CURRENT_KEYFRAME_SCHEMA_VERSION = 1;

Keyframe currentKeyframes[KEYFRAME_COUNT];

int defaultModifier(float fractionOfSolarDay, int minValue, int maxValue) {
  float progress = sinf(M_PI * fractionOfSolarDay);
  return minValue + (maxValue - minValue) * progress;
}

void clearStaleKeyframes(Preferences& prefs) {
  auto version = prefs.getUInt("version", 0);
  if (version != CURRENT_KEYFRAME_SCHEMA_VERSION) {
    prefs.clear();
    prefs.putUInt("version", CURRENT_KEYFRAME_SCHEMA_VERSION);
  }
}

void loadKeyframesFromStorage(Preferences& prefs) {
  char key[16];
  for (int i = 0; i < KEYFRAME_COUNT; i++) {
    snprintf(key, sizeof(key), "keyframe_%d", i);

    if (sizeof(Keyframe) != prefs.getBytes(key, (void*)&currentKeyframes[i], sizeof(Keyframe))) {
      float fractionOfSolarDay = (float)i / (KEYFRAME_COUNT - 1);
      currentKeyframes[i].fractionOfSolarDay = fractionOfSolarDay;
      currentKeyframes[i].colorTemperature = defaultModifier(fractionOfSolarDay, 2700, 6500);
      currentKeyframes[i].brightness = defaultModifier(fractionOfSolarDay, 25, 100);
    }
  }

  for (int i = 0; i < KEYFRAME_COUNT; i++) {
    Log.infoln("Loaded keyframe %d: { fractionOfSolarDay: %.3f, colorTemperature: %d, brightness: %d }",
      i,
      currentKeyframes[i].fractionOfSolarDay,
      currentKeyframes[i].colorTemperature,
      currentKeyframes[i].brightness);
    }
}

void TimeInputModule::Initialize() {
  Preferences prefs;
  prefs.begin("time_input");
  clearStaleKeyframes(prefs);
  loadKeyframesFromStorage(prefs);
  prefs.end();
}

void setKeyframesInMemory(const Keyframe keyframes[]) {
  for (int i = 0; i < KEYFRAME_COUNT; i++) {
    currentKeyframes[i].fractionOfSolarDay = keyframes[i].fractionOfSolarDay;
    currentKeyframes[i].colorTemperature = keyframes[i].colorTemperature;
    currentKeyframes[i].brightness = keyframes[i].brightness;
  }
}

void saveKeyframesToStorage(const Keyframe keyframes[]) {
  Preferences prefs;
  prefs.begin("time_input");

  char key[16];
  for (int i = 0; i < KEYFRAME_COUNT; i++) {
    snprintf(key, sizeof(key), "keyframe_%d", i);
    prefs.putBytes(key, (void*)&keyframes[i], sizeof(Keyframe));
  }

  prefs.end();
}

void TimeInputModule::SetKeyframes(const Keyframe keyframes[]) {
  setKeyframesInMemory(keyframes);
  saveKeyframesToStorage(keyframes);
}

void findSurroundingKeyframePair(float fractionOfSolarDay, Keyframe& lowerBound, Keyframe& upperBound) {
  lowerBound = currentKeyframes[0];
  upperBound = currentKeyframes[KEYFRAME_COUNT - 1];

  for (int i = 0; i < KEYFRAME_COUNT; i++) {
    if (currentKeyframes[i].fractionOfSolarDay <= fractionOfSolarDay) {
      lowerBound = currentKeyframes[i];
    }
    if (currentKeyframes[i].fractionOfSolarDay >= fractionOfSolarDay) {
      upperBound = currentKeyframes[i];
      break;
    }
  }
}

Keyframe interpolateKeyframes(const Keyframe& kf1, const Keyframe& kf2, float fractionOfSolarDay) {
  float range = kf2.fractionOfSolarDay - kf1.fractionOfSolarDay;
  float t = range == 0 ? 0 : (fractionOfSolarDay - kf1.fractionOfSolarDay) / range;

  Keyframe result;
  result.fractionOfSolarDay = kf1.fractionOfSolarDay + t * (kf2.fractionOfSolarDay - kf1.fractionOfSolarDay);
  result.colorTemperature = kf1.colorTemperature + t * (kf2.colorTemperature - kf1.colorTemperature);
  result.brightness = kf1.brightness + t * (kf2.brightness - kf1.brightness);
  return result;
}

Keyframe TimeInputModule::CurrentKeyframe(float fractionOfSolarDay) {
  Keyframe kf1;
  Keyframe kf2;
  findSurroundingKeyframePair(fractionOfSolarDay, kf1, kf2);

  if (kf1.fractionOfSolarDay == kf2.fractionOfSolarDay) {
    return kf1;
  } else {
    return interpolateKeyframes(kf1, kf2, fractionOfSolarDay);
  }
}

Keyframe* TimeInputModule::CurrentKeyframes() { return currentKeyframes; }
