#include "ManualInputModule.h"
#include <Arduino.h>

const int COLOR_TEMP_PIN = 34;
const int BRIGHTNESS_PIN = 35;

void ManualInputModule::Initialize() {
  pinMode(COLOR_TEMP_PIN, INPUT);
  pinMode(BRIGHTNESS_PIN, INPUT);
}

Light LightFromPotentiometers() {
  Light light;
  int colorTemp = analogRead(COLOR_TEMP_PIN);    // 0-4095 range on ESP32
  int brightness = analogRead(BRIGHTNESS_PIN);    // 0-4095 range on ESP32
  
  // Convert color temperature to warm/cool ratio (0.0 to 1.0)
  float ratio = colorTemp / 4095.0f;
  
  // Calculate warm and cool values based on the ratio and overall brightness
  light.warm = (1.0f - ratio) * brightness;
  light.cool = ratio * brightness;
  
  return light;
}


Light ManualInputModule::LightValues() {
  return LightFromPotentiometers();
}
