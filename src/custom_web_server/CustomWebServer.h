#pragma once
#include <ESPAsyncWebServer.h>

namespace CustomWebServer {
    void StartServerOnBackgroundCore();

    void ServerTask(void* parameter);
    void GetKeyframes(AsyncWebServerRequest *request);
    void PostKeyframes(AsyncWebServerRequest *request);
    void GetCurrentTime(AsyncWebServerRequest *request);
}
