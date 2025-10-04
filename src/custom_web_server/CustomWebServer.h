#pragma once
#include <WebServer.h>

namespace CustomWebServer {
    void StartServerOnBackgroundCore();

    void ServerTask(void* parameter);
    void SetupKeyframeApi(WebServer* server);
    void SetupCurrentTimeApi(WebServer* server);
}
