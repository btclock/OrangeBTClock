#include "config.hpp"

Preferences preferences;

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0;
const int daylightOffset_sec = 3600;

#define STA_SSID ""
#define STA_PASS ""

bool isUpdating = false;

void setupTime()
{
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void setupPreferences()
{
  preferences.begin("btclock", false);

  if (!preferences.isKey(SETTING_ROW1_CONTENT))
  {
    preferences.putUInt(SETTING_ROW1_CONTENT, LINE_BLOCKHEIGHT);
  }
  if (!preferences.isKey(SETTING_ROW2_CONTENT))
  {
    preferences.putUInt(SETTING_ROW2_CONTENT, LINE_SATSPERUNIT);
  }
  if (!preferences.isKey(SETTING_ROW3_CONTENT))
  {
    preferences.putUInt(SETTING_ROW3_CONTENT, LINE_MEMPOOL_FEES);
  }

  if (!preferences.isKey(SETTING_CURRENCY))
  {
    preferences.putString(SETTING_CURRENCY, CURRENCY_USD);
  }
}

void setupWifi()
{
  //  WiFi.begin(, ");
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);

  WiFiManager wm;

  // Touch pin 14 to reset
  if (touchRead(14) > 9000)
  {
    for (uint i = 0; i < 5; i++)
    {
      digitalWrite(LED_BUILTIN, LOW);
      delay(200);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(200);
    }

    if (touchRead(14) > 9000)
    {
      wm.resetSettings();
    }
  }

  byte mac[6];
  WiFi.macAddress(mac);
  String softAP_SSID =
      String("OrangeBTClock");
  WiFi.setHostname(softAP_SSID.c_str());
  String softAP_password =
      base64::encode(String(mac[2], 16) + String(mac[4], 16) +
                     String(mac[5], 16) + String(mac[1], 16))
          .substring(2, 10);

  // wm.setConfigPortalTimeout(preferences.getUInt("wpTimeout", 600));
  wm.setWiFiAutoReconnect(false);
  wm.setDebugOutput(false);
  wm.setConfigPortalBlocking(true);

  wm.setAPCallback([&](WiFiManager *wifiManager)
                   {
                     Serial.printf("Entered config mode:ip=%s, ssid='%s', pass='%s'\n",
                                   WiFi.softAPIP().toString().c_str(),
                                   wifiManager->getConfigPortalSSID().c_str(),
                                   softAP_password.c_str());
                     // delay(6000);

                     const String qrText = "qrWIFI:S:" + wifiManager->getConfigPortalSSID() +
                                           ";T:WPA;P:" + softAP_password.c_str() + ";;";
                     const String explainText = "SSID: " +
                                                wifiManager->getConfigPortalSSID() +
                                                "\r\nPassword: " + softAP_password;
                     showSetupText(explainText);
                     //        setEpdContent(epdContent);
                   });

  wm.setSaveConfigCallback([]()
                           {
        preferences.putBool("wifiConfigured", true);

        delay(1000);
        // just restart after succes
        ESP.restart(); });

  bool ac = wm.autoConnect(softAP_SSID.c_str(), softAP_password.c_str());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  // WiFi.setTxPower(WIFI_POWER_8_5dBm);
  //  enableWiFi();
}

void setModemSleep()
{
  digitalWrite(LED_BUILTIN, LOW);

  WiFi.setSleep(true);
  if (!setCpuFrequencyMhz(80))
  {
    Serial.println("Not valid frequency!");
  }
  // Use this if 40Mhz is not supported
  // setCpuFrequencyMhz(80);
}

void wakeModemSleep()
{
  setCpuFrequencyMhz(240);
  enableWiFi();
  digitalWrite(LED_BUILTIN, HIGH);
}

void enableWiFi()
{
  adc_power_on();
  delay(200);

  WiFi.disconnect(false); // Reconnect the network
  WiFi.mode(WIFI_STA);    // Switch WiFi off

  delay(200);

  Serial.println("START WIFI");
  WiFi.begin(STA_SSID, STA_PASS);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void disableWiFi()
{
  adc_power_off();
  WiFi.disconnect(true); // Disconnect from the network
  WiFi.mode(WIFI_OFF);   // Switch WiFi off
  Serial.println("");
  Serial.println("WiFi disconnected!");
}

bool inPowerSaveMode()
{
  return preferences.getBool("powerSaveMode", false);
}

void setupOTA()
{
  ArduinoOTA
      .onStart([]()
               {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type); 
      showSetupText("Updating...");
      isUpdating = true; })
      .onEnd([]()
             { Serial.println("\nEnd"); })
      .onProgress([](unsigned int progress, unsigned int total)
                  { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); })
      .onError([](ota_error_t error)
               {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed"); });

  ArduinoOTA.begin();
}