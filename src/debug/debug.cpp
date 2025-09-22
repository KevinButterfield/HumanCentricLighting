#include <Arduino.h>

const int LED_PIN = 2;

void BlinkingLight::Initialize() { 
    pinMode(LED_PIN, OUTPUT); 
}

void BlinkingLight::Flash() {
  for (int i = 0; i < 5; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(100);
  }
}