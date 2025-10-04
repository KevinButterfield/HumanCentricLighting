#include "TimekeepingModule.h"
#include "../custom_debug_utils/BlinkingLight.h"
#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoLog.h>

const char* const TZ_INFO = "EST5EDT,M3.2.0,M11.1.0";

bool TimekeepingModule::begin() {
  this->rtcConnected = rtc.begin();

  if (!this->rtcConnected) {
    Log.warningln(F("RTC not found - check connections"));
  } else if (rtc.lostPower()) {
    Log.warningln(F("RTC lost power, falling back to WiFi time"));
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
    Log.noticeln(F("Starting NTP time sync..."));
    configTzTime(TZ_INFO, "pool.ntp.org", "time.nist.gov");
    timeState = TIME_PENDING;
  }
}

void TimekeepingModule::syncRTCIfWifiSyncComplete() {
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    Log.noticeln(F("NTP time received: %d-%d-%d %d:%d:%d"),
               timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday,
               timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);

    if (rtcConnected) {
      time_t now;
      time(&now);
      rtc.adjust(DateTime(now));
      Log.noticeln(F("RTC synchronized with NTP time"));
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

  Log.noticeln(F("RTC time set: %d-%d-%d %d:%d:%d"),
             timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday,
             timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);

  timeState = TIME_SET;
  BlinkingLight::Toggle(true);
  return true;
}