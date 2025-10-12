#pragma once
#include <time.h>
#include <RTClib.h>

class TimekeepingModule {
public:
    TimekeepingModule() = default;

    bool begin();
    bool haveTime();
    DateTime now();

    void update();

private:
    enum TimeState { TIME_NONE, TIME_PENDING, TIME_SET };

    void startWifiTimeSyncIfConnected();
    void syncRTCIfWifiSyncComplete();
    bool setSystemTimeFromRTC();

    RTC_DS3231 rtc;
    TimeState timeState = TIME_NONE;
    bool rtcConnected = false; 
};