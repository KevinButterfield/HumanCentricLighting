#include "ManualInputModule.h"
#include <Arduino.h>
#include <ArduinoLog.h>

const int COLOR_TEMP_PIN = 34;
const int BRIGHTNESS_PIN = 35;

void ManualInputModule::Initialize() {
  pinMode(COLOR_TEMP_PIN, INPUT);
  pinMode(BRIGHTNESS_PIN, INPUT);
}

inline float analogRatio(int pin, int maxValue) {
  return analogRead(COLOR_TEMP_PIN) / 4095.0f * maxValue;
}

Light ManualInputModule::LightValues() {
  float colorTempValue = analogRead(COLOR_TEMP_PIN) / 4095.0f;
  float brightnessValue = analogRead(BRIGHTNESS_PIN) / 4095.0f;
  Log.verbose("Analog reads %F %F; ", colorTempValue, brightnessValue);

  Light light;

  light.colorTemperature = colorTempValue * (6500-2700) + 2700;
  light.brightness = brightnessValue;

  return light;
}
