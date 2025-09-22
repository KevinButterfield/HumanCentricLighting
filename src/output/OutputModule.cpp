#include "OutputModule.h"

#include <Arduino.h>
#include <RTClib.h>

const int WARM_PIN = 25;
const int COOL_PIN = 26;

void OutputModule::Initialize() {
  // Configure LEDC for PWM
  ledcSetup(0, 5000, 8);  // Channel 0, 5kHz, 8-bit resolution
  ledcAttachPin(WARM_PIN, 0);
  pinMode(COOL_PIN, OUTPUT);  // Keep cool pin simple for now
}

void OutputModule::Update(const DateTime& now) {
  // Create a sine wave that cycles every minute
  float phase = (now.second() * 1000 + millis() % 1000) / 60000.0f * 2 * PI;
  float sineValue = sin(phase);
  
  // Convert from -1..1 to 0..255 range for PWM
  int pwmValue = (sineValue + 1) * 127.5f;
  
  ledcWrite(0, pwmValue);
  
  // Print the value every update for monitoring
  Serial.printf("Time: %02d:%02d:%02d.%03d - PWM: %d\n", 
    now.hour(), 
    now.minute(), 
    now.second(),
    millis() % 1000,
    pwmValue
  );
}