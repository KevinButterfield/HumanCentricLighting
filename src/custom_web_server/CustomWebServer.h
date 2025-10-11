#pragma once
#include <ESPAsyncWebServer.h>

namespace CustomWebServer {
    void ConnectToWiFi();
    void StartServer();

    void SetupKeyframesApi(AsyncWebServer& server);
    void SetupCurrentTimeApi(AsyncWebServer& server);
}
