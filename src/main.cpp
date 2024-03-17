#include <Arduino.h>
#include <WebServer.h>

#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include "shared.hpp"
#include "epd.hpp"
#include "config.hpp"
#include "webserver.hpp"
#include <data.hpp>

#define uS_TO_S_FACTOR 1000000 /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP 5        /* Time ESP32 will go to sleep (in seconds) */
#define USE_SERIAL Serial

#ifdef ARDUINO_LOLIN_S2_MINI
GxEPD2_BW<EPD_CLASS, EPD_CLASS::HEIGHT> display = EPD_CLASS(4, 2, 3, 1);
#endif

#ifdef ARDUINO_LOLIN_S3_MINI
GxEPD2_BW<EPD_CLASS, EPD_CLASS::HEIGHT> display = EPD_CLASS(5, 3, 2, 1);
#endif

typedef void (*MethodPtr)(String, char);

MethodPtr methods[] = {nullptr, updateRow1, updateRow2, updateRow3};

WiFiClientSecure client;
uint currentPrice = 0;
String currentBlock = "";
String currentFees = "";

void setup()
{
  //  setCpuFrequencyMhz(40);
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(LED_BUILTIN, HIGH);

  setupPreferences();
  setupDisplay();

  setupWifi();
  setupTime();

  if (!inPowerSaveMode())
  {
    setupWebserver();
    setupOTA();
  }
  client.setInsecure();

  digitalWrite(LED_BUILTIN, LOW);
}

void loop()
{
  if (isUpdating)
  {
    delay(1000);
    return;
  }

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    setupTime();

    return;
  }
  client.setInsecure();

  //

  IPAddress res;
  uint result = WiFi.hostByName("mempool.space", res);

  if (result >= 0)
  {
    Serial.print("SUCCESS!");
    Serial.println(res.toString());
  }
  else
  {
    WiFi.reconnect();

    while (WiFi.status() != WL_CONNECTED)
    {
      Serial.print('.');
      delay(1000);
    }
  }

  for (uint i = 1; i <= 3; i++)
  {
    String rowContent = "";
    char icon;
    char keyName[5];
    snprintf(keyName, sizeof(keyName), "row%d", i);
    switch (preferences.getUInt(keyName))
    {
    case LINE_BLOCKHEIGHT:
      icon = ICON_BLOCK;
      rowContent = getBlock();
      break;
    case LINE_MEMPOOL_FEES:
      icon = ICON_PIE;
      rowContent = getMempoolFees();
      break;
    case LINE_MEMPOOL_FEES_MEDIAN:
      icon = ICON_PIE;
      rowContent = "NOT IMPL";
      break;
    case LINE_HALVING_COUNTDOWN:
    {
      icon = ICON_HOURGLASS;
      uint currentBlock = getBlock();
      rowContent = 210000 - (currentBlock % 210000);
      break;
    }
    case LINE_SATSPERUNIT:
    {
      icon = ICON_SATS;
      uint satsPerDollar = int(round(1 / float(getPrice()) * 10e7));
      rowContent = satsPerDollar;
      break;
    }
    case LINE_FIATPRICE:
      icon = getCurrencyIcon();
      rowContent = getPrice();
      break;
    case LINE_MARKETCAP:
    {
      icon = getCurrencyIcon();
      int64_t marketCap = static_cast<std::int64_t>(getSupplyAtBlock(getBlock()) * double(getPrice()));
      rowContent = String(formatNumberWithSuffix(marketCap, 8));
      break;
    }
    case LINE_TIME:
    {
      icon = ICON_GLOBE;
      char dateString[16];
      strftime(dateString, sizeof(dateString), preferences.getString(SETTING_TIME_FORMAT).c_str(), &timeinfo);
      rowContent = dateString;
      break;
    }
    case LINE_DATE:
    {
      icon = ICON_GLOBE;
      char dateString[16];
      strftime(dateString, sizeof(dateString), preferences.getString(SETTING_DATE_FORMAT).c_str(), &timeinfo);
      rowContent = dateString;
      break;
    }
    default:
      rowContent = "DEFAULT";
    }

    methods[i](rowContent, icon);
  }

  // String block = String(getBlock());

  // uint tryCount = 0;
  // while (block.equals(""))
  // {
  //   block = getBlock();
  //   Serial.print("Retry block..");
  //   tryCount++;

  //   Serial.println(tryCount);
  //   delay(1000);

  //   if (tryCount % 5)
  //   {
  //     WiFi.disconnect();
  //     WiFi.reconnect();

  //     while (WiFi.status() != WL_CONNECTED)
  //     {
  //       Serial.print('.');
  //       delay(1000);
  //     }
  //   }
  // }

  // uint price = getPrice();
  // tryCount = 0;
  // while (price == 0)
  // {
  //   price = getPrice();
  //   if (Serial.available())
  //     Serial.print("Retry price..");
  //   tryCount++;
  //   if (Serial.available())
  //     Serial.println(tryCount);
  //   delay(1000);
  // }

  // uint satsPerDollar = int(round(1 / float(price) * 10e7));

  // String mempoolFees = getMempoolFees();
  // tryCount = 0;
  // while (mempoolFees.equals(""))
  // {
  //   mempoolFees = getMempoolFees();
  //   Serial.print("Retry mempoolfees..");
  //   tryCount++;

  //   Serial.println(tryCount);
  //   delay(1000);
  // }

  // if (!currentFees.equals(mempoolFees))
  // {
  //   updateRow1(mempoolFees);
  //   currentFees = mempoolFees;
  //   Serial.print(F("Fees is now "));
  //   Serial.println(currentFees);
  // }
  // else
  // {
  //   Serial.println(F("No need to update fees"));
  // }

  // if (price != currentPrice)
  // {
  //   updateRow3(String(satsPerDollar));
  //   currentPrice = price;
  //   Serial.print(F("Price is now "));
  //   Serial.println(currentPrice);
  // }
  // else
  // {
  //   Serial.println(F("No need to update price"));
  // }

  // if (!block.equals(currentBlock))
  // {
  //   updateRow2(block);
  //   currentBlock = block;
  //   Serial.print(F("Block is now "));
  //   Serial.println(currentBlock);
  // }
  // else
  // {
  //   Serial.println(F("No need to update block"));
  // }

  // updateRows(mempoolFees, block, String(price));

  delay(2 * 1000);

  if (inPowerSaveMode())
  {
    display.hibernate();
    setModemSleep();
    esp_sleep_enable_timer_wakeup(50 * 1000000);
    esp_light_sleep_start();
    display.init(0, false);
    wakeModemSleep();
  }
  else
  {
    Serial.println(F("Sleeping"));
    sleep(50);
    //    delay(50 * 1000);
    Serial.println(F("Waking up"));
  }
}
