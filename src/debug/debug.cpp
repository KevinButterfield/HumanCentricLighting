#include <Arduino.h>

const int LED_PIN = 2;
bool ledState = false;

void BlinkingLight::Initialize() { 
    pinMode(LED_PIN, OUTPUT); 
}

void BlinkingLight::Toggle(bool state) {
  ledState = state;
  digitalWrite(LED_PIN, ledState ? HIGH : LOW);
}

void BlinkingLight::Toggle() {
  BlinkingLight::Toggle(!ledState);
}

void BlinkingLight::Flash() {
  for (int i = 0; i < 5; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(100);
  }
  digitalWrite(LED_PIN, ledState ? HIGH : LOW);
}