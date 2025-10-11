#include "TimeInputModule.h"

#include <ArduinoLog.h>
#include <Preferences.h>
#include <math.h>
#include <timekeeping/SolarTime.h>

void findSurroundingKeyframePair(float fractionOfSolarDay, Keyframe &lowerBound, Keyframe &upperBound)
{
  auto currentKeyframes = TimeInputModule::CurrentKeyframesInternal();
  lowerBound = currentKeyframes[0];
  upperBound = currentKeyframes[KEYFRAME_COUNT - 1];

  for (int i = 0; i < KEYFRAME_COUNT; i++)
  {
    if (currentKeyframes[i].fractionOfSolarDay <= fractionOfSolarDay)
    {
      lowerBound = currentKeyframes[i];
    }
    if (currentKeyframes[i].fractionOfSolarDay >= fractionOfSolarDay)
    {
      upperBound = currentKeyframes[i];
      break;
    }
  }
}

Keyframe interpolateKeyframes(const Keyframe &kf1, const Keyframe &kf2, float fractionOfSolarDay)
{
  float range = kf2.fractionOfSolarDay - kf1.fractionOfSolarDay;
  float t = range == 0 ? 0 : (fractionOfSolarDay - kf1.fractionOfSolarDay) / range;

  Keyframe result;
  result.fractionOfSolarDay = kf1.fractionOfSolarDay + t * (kf2.fractionOfSolarDay - kf1.fractionOfSolarDay);
  result.colorTemperature = kf1.colorTemperature + t * (kf2.colorTemperature - kf1.colorTemperature);
  result.brightness = kf1.brightness + t * (kf2.brightness - kf1.brightness);
  return result;
}

Keyframe currentKeyframe(float fractionOfSolarDay)
{
  Keyframe kf1;
  Keyframe kf2;
  findSurroundingKeyframePair(fractionOfSolarDay, kf1, kf2);

  if (kf1.fractionOfSolarDay == kf2.fractionOfSolarDay)
  {
    return kf1;
  }
  else
  {
    return interpolateKeyframes(kf1, kf2, fractionOfSolarDay);
  }
}

Light TimeInputModule::LightValues() {
  auto now = SolarTime::Now();
  Keyframe keyframe = currentKeyframe(now.currentFraction);
  Log.verbose("Time read SolarFraction: %F; ", now.currentFraction);

  Light output;
  output.brightness = keyframe.brightness;
  output.colorTemperature = keyframe.colorTemperature;

  return output;
}