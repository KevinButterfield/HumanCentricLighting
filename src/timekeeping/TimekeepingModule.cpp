#include <Arduino.h>
#include <WiFi.h>
#include "TimekeepingModule.h"

const char* const TZ_INFO = "EST5EDT,M3.2.0,M11.1.0";

void TimekeepingModule::update() {
    if (!timeInitialized && WiFi.status() == WL_CONNECTED) {
        Serial.println("Starting time sync...");
        configTzTime(TZ_INFO, "pool.ntp.org", "time.nist.gov");
        Serial.println("Time sync started.");
        struct tm timeinfo;
        if (getLocalTime(&timeinfo)) {
            Serial.println(&timeinfo, "Current time: %Y-%m-%d %H:%M:%S");
            timeInitialized = true;
            BlinkingLight::Flash();
        }
    }
}

time_t TimekeepingModule::getCurrentTime() {
    if (timeInitialized) {
        return time(nullptr);
    } else {
        return 0;
    }
}