#include "OutputModule.h"

#include <Arduino.h>
#include <ArduinoLog.h>
#include <RTClib.h>

const int WARM_CHANNEL = 0;
const int WARM_PIN = 25;

const int COOL_CHANNEL = 1;
const int COOL_PIN = 26;

const int RESOLUTION_BITS = 12;
const int FREQUENCY_HZ = 5000;

void OutputModule::Initialize()
{
  ledcSetup(WARM_CHANNEL, FREQUENCY_HZ, RESOLUTION_BITS);
  ledcSetup(COOL_CHANNEL, FREQUENCY_HZ, RESOLUTION_BITS);
  ledcAttachPin(WARM_PIN, WARM_CHANNEL);
  ledcAttachPin(COOL_PIN, COOL_CHANNEL);
}

void OutputModule::Update(const Light &light)
{
  const float T_min = 2700.0;
  const float T_max = 6500.0;

  float w = (T_max - light.colorTemperature) / (T_max - T_min);
  float c = (light.colorTemperature - T_min) / (T_max - T_min);

  float maxVal = max(w, c);

  int warm = (w / maxVal) * 4095.0 * light.brightness;
  int cool = (c / maxVal) * 4095.0 * light.brightness;

  Log.verboseln(F("PWM update - Brightness: %F, ColorTemp: %d, Warm: %d, Cool: %d"), light.brightness, light.colorTemperature, warm, cool);
  ledcWrite(WARM_CHANNEL, warm);
  ledcWrite(COOL_CHANNEL, cool);
}