#include "InputModule.h"
#include <Arduino.h>

const int WARM_INPUT_PIN = 34;
const int COOL_INPUT_PIN = 35;

void InputModule::Initialize() {
  pinMode(WARM_INPUT_PIN, INPUT);
  pinMode(COOL_INPUT_PIN, INPUT);
}

Light InputModule::LightValues() {
  Light light;
  light.warm = analogRead(WARM_INPUT_PIN);
  light.cool = analogRead(COOL_INPUT_PIN);
  return light;
}
