#include "InputModule.h"
#include <Arduino.h>

const int WARM_INPUT_PIN = 34;
const int COOL_INPUT_PIN = 35;

void InputModule::Initialize() {
  pinMode(WARM_INPUT_PIN, INPUT);
  pinMode(COOL_INPUT_PIN, INPUT);
}

int InputModule::WarmValue() {
  return analogRead(WARM_INPUT_PIN);
}

int InputModule::CoolValue() {
  return analogRead(COOL_INPUT_PIN);
}