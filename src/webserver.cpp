#include "webserver.hpp"
#include <LittleFS.h>

AsyncWebServer server(80);
const String uintSettings[] = {SETTING_ROW1_CONTENT, SETTING_ROW2_CONTENT, SETTING_ROW3_CONTENT};
const String stringSettings[] = {SETTING_CURRENCY};
const String boolSettings[] = {};

void setupWebserver()
{
  if (!LittleFS.begin(true))
  {
    Serial.println(F("An Error has occurred while mounting LittleFS"));
  }

  server.on("/api/settings", HTTP_GET, onApiSettingsGet);
  AsyncCallbackJsonWebHandler *settingsPatchHandler =
      new AsyncCallbackJsonWebHandler("/api/json/settings", onApiSettingsPatch);
  server.addHandler(settingsPatchHandler);

  server.serveStatic("/build", LittleFS, "/build");

  server.on("/", HTTP_GET, onIndex);

  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods",
                                       "GET, PATCH, POST, OPTIONS");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "*");

  server.begin();
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
