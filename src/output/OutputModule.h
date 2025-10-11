#pragma once
#include <RTClib.h>

struct Light {
    int colorTemperature;
    float brightness;
};

namespace OutputModule {
    void Initialize();
    void Update(const Light& light);
}