#pragma once

// Keep order of includes because of conflicts
#include "ESPAsyncWebServer.h"
#include <ArduinoJson.h>

void setupWebserver();
void onIndex(AsyncWebServerRequest *request);
void onNotFound(AsyncWebServerRequest *request);