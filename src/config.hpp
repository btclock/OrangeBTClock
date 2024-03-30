#pragma once

#include <Arduino.h>
#include <Preferences.h>
#include "shared.hpp"
#include "driver/adc.h"
#include <WiFiManager.h>
#include <base64.h>
#include "epd.hpp"
#include <ArduinoOTA.h>

void enableWiFi();
void disableWiFi();
void setupTime();
void setupPreferences();
void setupWifi();
void setupOTA();
void OTAUpdateTask(void *pvParameters);

void wakeModemSleep();
void setModemSleep();

bool inPowerSaveMode();
char getCurrencyIcon();
void IRAM_ATTR onButtonPress();
void setupButtonISR();