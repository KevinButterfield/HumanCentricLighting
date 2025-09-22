#include <time.h>

class TimekeepingModule {
public:
    TimekeepingModule() = default;

    void update();
    time_t getCurrentTime();

private:
    bool timeInitialized = false;
};