#include "OutputModule.h"

#include <Arduino.h>
#include <RTClib.h>

const int WARM_CHANNEL = 0;
const int WARM_PIN = 25;
const int COOL_CHANNEL = 1;
const int COOL_PIN = 26;

void OutputModule::Initialize() {
  ledcSetup(WARM_CHANNEL, 5000, 12);  // Channel 0, 5kHz, 12-bit resolution
  ledcAttachPin(WARM_PIN, WARM_CHANNEL);
  pinMode(COOL_PIN, OUTPUT);  // Keep cool pin simple for now
}

void OutputModule::Update(const int value) {
  Serial.printf("Setting warm LED duty cycle to %d (out of 4095)\n", value);
  ledcWrite(WARM_CHANNEL, value);
}