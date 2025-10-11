#include "TimeInputModule.h"

#include <ArduinoLog.h>
#include <Preferences.h>
#include <sstream>
#include <math.h>

constexpr int CURRENT_KEYFRAME_SCHEMA_VERSION = 3;

Keyframe currentKeyframes[KEYFRAME_COUNT];

Keyframe *TimeInputModule::CurrentKeyframesInternal()
{
  return currentKeyframes;
}

float defaultModifier(float fractionOfSolarDay, float minValue, float maxValue)
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
    currentKeyframes[i].fractionOfSolarDay = json[i]["fractionOfSolarDay"].as<float>();
    currentKeyframes[i].colorTemperature = json[i]["colorTemperature"].as<int>();
    currentKeyframes[i].brightness = json[i]["brightness"].as<float>();
  }
}

void setCurrentToSinWaveDefault()
{
  for (int i = 0; i < KEYFRAME_COUNT; ++i)
  {
    float fractionOfSolarDay = (float)i / (KEYFRAME_COUNT - 1);
    currentKeyframes[i].fractionOfSolarDay = fractionOfSolarDay;
    currentKeyframes[i].colorTemperature = defaultModifier(fractionOfSolarDay, 2700, 6500);
    currentKeyframes[i].brightness = defaultModifier(fractionOfSolarDay, .25, 1);
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
    Log.infoln(F("Loaded stored keyframes %s"), json);
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

std::vector<Keyframe> TimeInputModule::CurrentKeyframes()
{
  auto currentKeyframes = CurrentKeyframesInternal();
  return std::vector<Keyframe>(currentKeyframes, currentKeyframes + KEYFRAME_COUNT);
}

void TimeInputModule::Initialize()
{
  Preferences prefs;
  prefs.begin("time_input");
  clearStaleKeyframes(prefs);
  loadKeyframesFromStorage(prefs);
  prefs.end();
}

bool validateFraction(JsonVariant value, float min) {
  return value.is<float>() && value.as<float>() >= min && value.as<float>() <= 1;
}

bool validateColorTemp(JsonVariant value) {
  return value.is<int>() && value.as<int>() >= 2700 && value.as<int>() <= 6500;
}

String TimeInputModule::ValidateKeyframe(const JsonObject json)
{
  bool fractionGood = validateFraction(json["fractionOfSolarDay"], 0);
  bool colorTempGood = validateColorTemp(json["colorTemperature"]);
  bool brightnessGood = validateFraction(json["brightness"], 0.25);
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