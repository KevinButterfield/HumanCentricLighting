#include "TimekeepingModule.h"

#include <Arduino.h>
#include <WiFi.h>

const char* const TZ_INFO = "EST5EDT,M3.2.0,M11.1.0";

bool TimekeepingModule::begin() {
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    return false;
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, falling back to WiFi");
  } else {
    setSystemTimeFromRTC();
  }

  return true;
}

void TimekeepingModule::update() {
  if (!timeInitialized && WiFi.status() == WL_CONNECTED) {
    setRTCFromNTP();
  }
}

DateTime TimekeepingModule::now() {
  if (timeInitialized) {
    return rtc.now();
  } else {
    return DateTime();
  }
}

bool TimekeepingModule::setSystemTimeFromRTC() {
  DateTime now = rtc.now();

  struct tm timeinfo;
  timeinfo.tm_year = now.year() - 1900;
  timeinfo.tm_mon = now.month() - 1;
  timeinfo.tm_mday = now.day();
  timeinfo.tm_hour = now.hour();
  timeinfo.tm_min = now.minute();
  timeinfo.tm_sec = now.second();
  time_t t = mktime(&timeinfo);

  struct timeval tv = {.tv_sec = t};
  settimeofday(&tv, NULL);

  Serial.println(&timeinfo, "Current time: %Y-%m-%d %H:%M:%S");

  timeInitialized = true;
  BlinkingLight::Toggle(true);
  return true;
}

bool TimekeepingModule::setRTCFromNTP() {
  Serial.println("Starting time sync...");
  configTzTime(TZ_INFO, "pool.ntp.org", "time.nist.gov");

  struct tm timeinfo;
  while (!getLocalTime(&timeinfo)) {
    Serial.println("Waiting for NTP time sync...");
    delay(500);
  }

  time_t now;
  time(&now);
  rtc.adjust(DateTime(now));
  Serial.println("RTC synchronized with NTP time");
  Serial.println(&timeinfo, "Current time: %Y-%m-%d %H:%M:%S");

  timeInitialized = true;
  BlinkingLight::Toggle(true);
  return true;
}
