#include "InputModule.h"
#include <Arduino.h>

const int POTENTIOMETER_PIN = 34;

void InputModule::Initialize() {
  pinMode(POTENTIOMETER_PIN, INPUT);
}

void InputModule::ReadPotentiometer() {
  int value = analogRead(POTENTIOMETER_PIN);
  Serial.printf("Potentiometer value: %d\n", value);
}