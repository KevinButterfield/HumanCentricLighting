#include "SolarTime.h"
#include <SolarCalculator.h>
#include <Arduino.h>
#include <ArduinoLog.h>

constexpr double LATITUDE = 39.7910;
constexpr double LONGITUDE = -86.1480;

float calculateCurrentFraction(const tm& timeinfo, const double sunriseHours, const double sunsetHours) {
  double currentTimeHours = timeinfo.tm_hour + timeinfo.tm_min / 60.0 + timeinfo.tm_sec / 3600.0;
  Log.verbose("currentTimeHours %F sunriseHours %F sunsetHours %F; ", currentTimeHours, sunriseHours, sunsetHours);

  if (currentTimeHours < sunriseHours) {
    return 0.0;
  } else if (currentTimeHours > sunsetHours) {
    return 1.0;
  } else {
    return (currentTimeHours - sunriseHours) / (sunsetHours - sunriseHours);
  }
}

SolarTime SolarTime::Now() {
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

  SolarTime output;
  output.sunriseHours = sunriseHours;
  output.sunsetHours = sunsetHours;
  output.currentFraction = calculateCurrentFraction(timeinfo, sunriseHours, sunsetHours);

  return output;
}
