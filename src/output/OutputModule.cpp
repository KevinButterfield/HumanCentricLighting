#include "OutputModule.h"

#include <Arduino.h>
#include <ArduinoLog.h>
#include <RTClib.h>

const int WARM_CHANNEL = 0;
const int WARM_PIN = 32;

const int COOL_CHANNEL = 1;
const int COOL_PIN = 26;

const int RESOLUTION_BITS = 12;
const int FREQUENCY_HZ = 5000;

void OutputModule::Initialize()
{
  ledcSetup(WARM_CHANNEL, FREQUENCY_HZ, RESOLUTION_BITS);
  ledcSetup(COOL_CHANNEL, FREQUENCY_HZ, RESOLUTION_BITS);
  ledcAttachPin(WARM_PIN, WARM_CHANNEL);
  ledcAttachPin(COOL_PIN, COOL_CHANNEL);
}

// PWMValues calculateWarmCoolPWM(float T, float B, float gamma = 2.2f, uint32_t max_pwm = 255) {
//   const float Tmin = 2700.0f;
//   const float Tmax = 6500.0f;

//   // clamp inputs
//   if (T < Tmin) T = Tmin;
//   if (T > Tmax) T = Tmax;
//   if (B < 0.0f) B = 0.0f;
//   if (B > 1.0f) B = 1.0f;
//   if (gamma <= 0.0f) gamma = 1.0f; // protect

//   // unit mix factors (linear)
//   float w = (Tmax - T) / (Tmax - Tmin); // warm fraction (1 at Tmin)
//   float c = (T - Tmin) / (Tmax - Tmin); // cool fraction (1 at Tmax)

//   // avoid division by zero (shouldn't happen with valid Tmin/Tmax)
//   float maxVal = (w > c) ? w : c;
//   if (maxVal <= 0.0f) maxVal = 1.0f;

//   // Normalize so the maximum channel is 1.0 at B==1.0
//   float warm_lin = (w / maxVal) * B; // in [0..1]
//   float cool_lin = (c / maxVal) * B;

//   // Apply perceptual gamma correction:
//   // PWM = (linear_intensity)^(1/gamma) * max_pwm
//   float invGamma = 1.0f / gamma;
//   float warm_pwm_f = powf(warm_lin, invGamma) * (float)max_pwm;
//   float cool_pwm_f = powf(cool_lin, invGamma) * (float)max_pwm;

//   // Round & clamp to integer range
//   uint32_t warm_pwm = (warm_pwm_f <= 0.0f) ? 0u : (uint32_t)roundf(fminf(warm_pwm_f, (float)max_pwm));
//   uint32_t cool_pwm = (cool_pwm_f <= 0.0f) ? 0u : (uint32_t)roundf(fminf(cool_pwm_f, (float)max_pwm));

//   return PWMValues{ warm_pwm, cool_pwm };
// }

void OutputModule::Update(const Light &light)
{
  const float T_min = 2700.0;
  const float T_max = 6500.0;

  float w = (T_max - light.colorTemperature) / (T_max - T_min);
  float c = (light.colorTemperature - T_min) / (T_max - T_min);

  float maxVal = max(w, c);

  int warm = (w / maxVal) * 4095.0 * light.brightness;
  int cool = (c / maxVal) * 4095.0 * light.brightness;

  Log.verboseln(F("PWM update - Brightness: %F, ColorTemp: %d, Warm: %d, Cool: %d"), light.brightness, light.colorTemperature, warm, cool);
  ledcWrite(WARM_CHANNEL, warm);
  ledcWrite(COOL_CHANNEL, cool);
}