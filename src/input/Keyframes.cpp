#include "TimeInputModule.h"

#include <ArduinoLog.h>
#include <Preferences.h>
#include <sstream>
#include <math.h>

constexpr int CURRENT_KEYFRAME_SCHEMA_VERSION = 1;

Keyframe currentKeyframes[KEYFRAME_COUNT];

Keyframe *TimeInputModule::CurrentKeyframesInternal()
{
  return currentKeyframes;
}

int defaultModifier(float fractionOfSolarDay, int minValue, int maxValue)
{
  float progress = sinf(M_PI * fractionOfSolarDay);
  return minValue + (maxValue - minValue) * progress;
}

void clearStaleKeyframes(Preferences &prefs)
{
  auto version = prefs.getUInt("version", 0);
  if (version != CURRENT_KEYFRAME_SCHEMA_VERSION)
  {
    prefs.clear();
    prefs.putUInt("version", CURRENT_KEYFRAME_SCHEMA_VERSION);
  }
}

void setCurrentFromJson(JsonArray json)
{
  for (int i = 0; i < KEYFRAME_COUNT; ++i)
  {
    float fractionOfSolarDay = json[i]["fractionOfSolarDay"];
    int colorTemperature = json[i]["colorTemperature"];
    int brightness = json[i]["brightness"];

    currentKeyframes[i].fractionOfSolarDay = fractionOfSolarDay;
    currentKeyframes[i].colorTemperature = colorTemperature;
    currentKeyframes[i].brightness = brightness;
  }
}

void setCurrentToSinWaveDefault()
{
  for (int i = 0; i < KEYFRAME_COUNT; ++i)
  {
    float fractionOfSolarDay = (float)i / (KEYFRAME_COUNT - 1);
    currentKeyframes[i].fractionOfSolarDay = fractionOfSolarDay;
    currentKeyframes[i].colorTemperature = defaultModifier(fractionOfSolarDay, 2700, 6500);
    currentKeyframes[i].brightness = defaultModifier(fractionOfSolarDay, 25, 100);
  }
}

void loadKeyframesFromStorage(Preferences &prefs)
{
  String json = prefs.getString("keyframes");
  JsonDocument parsed;

  if (json.isEmpty() || deserializeJson(parsed, json))
  {
    Log.infoln(F("No stored keyframes found, reverting to default"));
    setCurrentToSinWaveDefault();
  }
  else
  {
    Log.infoln(F("Loaded stored keyframes"));
    setCurrentFromJson(parsed.as<JsonArray>());
  }
}

void saveKeyframesToStorage(const String &serialized)
{
  Preferences prefs;
  prefs.begin("time_input");
  prefs.putString("keyframes", serialized);
  prefs.end();
}

void TimeInputModule::Initialize()
{
  Preferences prefs;
  prefs.begin("time_input");
  clearStaleKeyframes(prefs);
  loadKeyframesFromStorage(prefs);
  prefs.end();
}

String TimeInputModule::ValidateKeyframe(const JsonObject json)
{
  auto fractionGood = json["fractionOfSolarDay"].is<float>();
  auto colorTempGood = json["colorTemperature"].is<uint16_t>();
  auto brightnessGood = json["brightness"].is<uint8_t>();
  std::stringstream errors;

  if (!fractionGood) errors << "Invalid fractionOfSolarDay ";
  if (!colorTempGood) errors << "Invalid colorTemperature ";
  if (!brightnessGood) errors << "Invalid brightness ";

  std::string string(errors.str());
  return String(string.c_str());
}

void TimeInputModule::SetKeyframes(const JsonArray json, const String &raw)
{
  setCurrentFromJson(json);
  saveKeyframesToStorage(raw);
}