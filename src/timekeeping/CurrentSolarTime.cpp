#include "CurrentSolarTime.h"
#include <SolarCalculator.h>
#include <Arduino.h>
#include <ArduinoLog.h>

constexpr double LATITUDE = 39.7910;
constexpr double LONGITUDE = -86.1480;

float CurrentFractionOfSolarDay() {
  struct tm timeinfo;
  getLocalTime(&timeinfo);

  double transit, sunriseHours, sunsetHours;
  calcSunriseSunset(
    timeinfo.tm_year + 1900,
    timeinfo.tm_mon + 1,
    timeinfo.tm_mday,
    LATITUDE,
    LONGITUDE,
    transit,
    sunriseHours,
    sunsetHours
  );

  Log.infoln("Sunrise: %.2f, Transit: %.2f, Sunset: %.2f", sunriseHours, transit, sunsetHours);

  double currentTimeHours = timeinfo.tm_hour + timeinfo.tm_min / 60.0 + timeinfo.tm_sec / 3600.0;
  
  if (currentTimeHours < sunriseHours) {
    return 0.0;
  } else if (currentTimeHours > sunsetHours) {
    return 1.0;
  } else {
    return (currentTimeHours - sunriseHours) / (sunsetHours - sunriseHours);
  }
}
