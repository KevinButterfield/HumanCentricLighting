#include "TimekeepingModule.h"

#include <Arduino.h>
#include <WiFi.h>

const char* const TZ_INFO = "EST5EDT,M3.2.0,M11.1.0";

bool TimekeepingModule::begin() {
  this->rtcConnected = rtc.begin();

  if (!this->rtcConnected) {
    Serial.println("Couldn't find RTC");
  } else if (rtc.lostPower()) {
    Serial.println("RTC lost power, falling back to WiFi");
  } else {
    setSystemTimeFromRTC();
  }

  return true;
}

DateTime TimekeepingModule::now() {
  time_t now;
  time(&now);
  return DateTime(now);
}

void TimekeepingModule::update() {
  switch (timeState) {
    case TIME_NONE:
      startWifiTimeSyncIfConnected();
      break;
    case TIME_PENDING: {
      syncRTCIfWifiSyncComplete();
      break;
    case TIME_SET:
      // Nothing to do
      break;
    }
  }
}

void TimekeepingModule::startWifiTimeSyncIfConnected() {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Starting time sync from wifi...");
    configTzTime(TZ_INFO, "pool.ntp.org", "time.nist.gov");
    timeState = TIME_PENDING;
  }
}

void TimekeepingModule::syncRTCIfWifiSyncComplete() {
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    Serial.println(&timeinfo, "Current time from WiFi: %Y-%m-%d %H:%M:%S");

    if (rtcConnected) {
      time_t now;
      time(&now);
      rtc.adjust(DateTime(now));
    }

    timeState = TIME_SET;
    BlinkingLight::Toggle(true);
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

  Serial.println(&timeinfo, "Current time from RTC: %Y-%m-%d %H:%M:%S");

  timeState = TIME_SET;
  BlinkingLight::Toggle(true);
  return true;
}