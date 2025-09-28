#pragma once
#include <RTClib.h>

struct Light {
    int warm;
    int cool;
};

namespace OutputModule {
    void Initialize();
    void Update(const Light& light);
}