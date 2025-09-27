#include "InputModule.h"
#include <Arduino.h>

const int POTENTIOMETER_PIN = 34;

void InputModule::Initialize() {
  pinMode(POTENTIOMETER_PIN, INPUT);
}

int InputModule::ReadPotentiometer() {
  int value = analogRead(POTENTIOMETER_PIN);
  Serial.printf("Potentiometer value: %d\n", value);

  return value; // 0-4095
}