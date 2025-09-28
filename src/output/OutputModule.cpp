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

void OutputModule::Initialize() {
  ledcSetup(WARM_CHANNEL, FREQUENCY_HZ, RESOLUTION_BITS);
  ledcSetup(COOL_CHANNEL, FREQUENCY_HZ, RESOLUTION_BITS);
  ledcAttachPin(WARM_PIN, WARM_CHANNEL);
  ledcAttachPin(COOL_PIN, COOL_CHANNEL);
}

void OutputModule::Update(const Light& light) {
  Log.verboseln(F("PWM update - Warm: %d, Cool: %d"), light.warm, light.cool);
  ledcWrite(WARM_CHANNEL, light.warm);
  ledcWrite(COOL_CHANNEL, light.cool);
}