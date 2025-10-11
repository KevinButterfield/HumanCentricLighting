#pragma once
#include <ESPAsyncWebServer.h>

namespace CustomWebServer {
    void ConnectToWiFi();
    void StartServer();

    void GetKeyframes(AsyncWebServerRequest *request);
    void PostKeyframes(AsyncWebServerRequest *request, uint8_t* data, size_t len, size_t index, size_t total);
    void GetCurrentTime(AsyncWebServerRequest *request);
}
