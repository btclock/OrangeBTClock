#include "webserver.hpp"
#include <LittleFS.h>

AsyncWebServer server(80);

void setupWebserver() {
  if (!LittleFS.begin(true)) {
    Serial.println(F("An Error has occurred while mounting LittleFS"));
  }

  server.serveStatic("/build", LittleFS, "/build");

  server.on("/", HTTP_GET, onIndex);

  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods",
                                       "GET, PATCH, POST, OPTIONS");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "*");

  server.begin();
}

void onIndex(AsyncWebServerRequest *request) {
  request->send(LittleFS, "/index.html", String(), false);
}

void onNotFound(AsyncWebServerRequest *request) {
 if (request->method() == HTTP_OPTIONS ||
      request->hasHeader("Sec-Fetch-Mode")) {
    // Serial.printf("NotFound, Return[%d]\n", 200);

    request->send(200);
  } else {
    // Serial.printf("NotFound, Return[%d]\n", 404);
    request->send(404);
  }
}
