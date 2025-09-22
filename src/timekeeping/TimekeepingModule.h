#include <time.h>
#include <RTClib.h>

class TimekeepingModule {
public:
    TimekeepingModule() = default;

    bool begin();
    void update();
    DateTime now();

private:
    bool setSystemTimeFromRTC();
    bool setRTCFromNTP();

    RTC_DS3231 rtc;
    bool timeInitialized = false;
};