#include <time.h>
#include <RTClib.h>

class TimekeepingModule {
public:
    TimekeepingModule() = default;

    bool begin();  // New method for initialization
    void update();

private:
    bool setSystemTimeFromRTC();
    bool setRTCFromNTP();

    RTC_DS3231 rtc;
    bool timeInitialized = false;
};