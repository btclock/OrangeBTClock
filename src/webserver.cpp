#include "webserver.hpp"
#include <LittleFS.h>

AsyncWebServer server(80);
const String uintSettings[] = {SETTING_ROW1_CONTENT, SETTING_ROW2_CONTENT, SETTING_ROW3_CONTENT};
const String stringSettings[] = {SETTING_CURRENCY,SETTING_MEMPOOL_INSTANCE};
const String boolSettings[] = {};

void setupWebserver()
{
  if (!LittleFS.begin(true))
  {
    Serial.println(F("An Error has occurred while mounting LittleFS"));
  }

  server.on("/api/status", HTTP_GET, onApiStatus);
  server.on("/api/settings", HTTP_GET, onApiSettingsGet);
  server.on("/api/restart", HTTP_GET, onApiRestart);
  server.on("/api/full_refresh", HTTP_GET, onApiFullRefresh);

  AsyncCallbackJsonWebHandler *settingsPatchHandler =
      new AsyncCallbackJsonWebHandler("/api/json/settings", onApiSettingsPatch);
  server.addHandler(settingsPatchHandler);

  server.serveStatic("/build", LittleFS, "/build");

  server.on("/", HTTP_GET, onIndex);
  server.onNotFound(onNotFound);

  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods",
                                       "GET, PATCH, POST, OPTIONS");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "*");

  server.begin();
}

void onApiStatus(AsyncWebServerRequest *request)
{
  JsonDocument root;

  root["row1"] = currentRow1;
  root["row2"] = currentRow2;
  root["row3"] = currentRow3;

  root["icon1"] = String(currentIcon1);
  root["icon2"] = String(currentIcon2);
  root["icon3"] = String(currentIcon3);

  root["espUptime"] = esp_timer_get_time() / 1000000;
  root["espFreeHeap"] = ESP.getFreeHeap();
  root["espHeapSize"] = ESP.getHeapSize();

  root["rssi"] = WiFi.RSSI();

  AsyncResponseStream *response =
      request->beginResponseStream("application/json");
  serializeJson(root, *response);

  request->send(response);
}

void onApiSettingsGet(AsyncWebServerRequest *request)
{
  JsonDocument root;
  for (String setting : uintSettings)
  {
    root[setting] = preferences.getUInt(setting.c_str());
  }

  for (String setting : stringSettings)
  {
    root[setting] = preferences.getString(setting.c_str());
  }

  for (String setting : boolSettings)
  {
    root[setting] = preferences.getBool(setting.c_str());
  }

  root["hostname"] = getMyHostname();
  root["ip"] = WiFi.localIP();
  root["txPower"] = WiFi.getTxPower();

#ifdef GIT_REV
  root["gitRev"] = String(GIT_REV);
#endif
#ifdef LAST_BUILD_TIME
  root["lastBuildTime"] = String(LAST_BUILD_TIME);
#endif

  AsyncResponseStream *response =
      request->beginResponseStream("application/json");
  serializeJson(root, *response);

  request->send(response);
}

void onApiSettingsPatch(AsyncWebServerRequest *request, JsonVariant &json)
{
  JsonObject settings = json.as<JsonObject>();

  for (String setting : uintSettings)
  {
    if (settings.containsKey(setting))
    {
      preferences.putUInt(setting.c_str(), settings[setting].as<uint>());
      Serial.printf("Setting %s to %d\r\n", setting.c_str(),
                    settings[setting].as<uint>());
    }
  }

  for (String setting : stringSettings)
  {
    if (settings.containsKey(setting))
    {
      preferences.putString(setting.c_str(), settings[setting].as<String>());
      Serial.printf("Setting %s to %s\r\n", setting.c_str(),
                    settings[setting].as<String>());
    }
  }

  for (String setting : boolSettings)
  {
    if (settings.containsKey(setting))
    {
      preferences.putBool(setting.c_str(), settings[setting].as<boolean>());
      Serial.printf("Setting %s to %d\r\n", setting.c_str(),
                    settings[setting].as<boolean>());
    }
  }

  request->send(200);
}

void onIndex(AsyncWebServerRequest *request)
{
  request->send(LittleFS, "/index.html", String(), false);
}

void onNotFound(AsyncWebServerRequest *request)
{
  if (request->method() == HTTP_OPTIONS ||
      request->hasHeader("Sec-Fetch-Mode"))
  {
    // Serial.printf("NotFound, Return[%d]\n", 200);

    request->send(200);
  }
  else
  {
    // Serial.printf("NotFound, Return[%d]\n", 404);
    request->send(404);
  }
}

void onApiRestart(AsyncWebServerRequest *request) {
  request->send(200);

  delay(500);

  esp_restart();
}

/**
 * @Api
 * @Path("/api/full_refresh")
 */
void onApiFullRefresh(AsyncWebServerRequest *request) {
  display.refresh();

  request->send(200);
}