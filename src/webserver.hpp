#pragma once

// Keep order of includes because of conflicts
#include "ESPAsyncWebServer.h"
#include "AsyncJson.h"

#include <ArduinoJson.h>
#include <shared.hpp>

void setupWebserver();

void onApiStatus(AsyncWebServerRequest *request);

void onApiSettingsGet(AsyncWebServerRequest *request);
void onApiSettingsPatch(AsyncWebServerRequest *request, JsonVariant &json);

void onIndex(AsyncWebServerRequest *request);
void onNotFound(AsyncWebServerRequest *request);