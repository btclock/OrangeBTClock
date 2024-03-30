#pragma once

#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <Preferences.h>
#include <GxEPD2.h>
#include <GxEPD2_BW.h>
#include "utils.hpp"
#include "fonts/fonts.hpp"
#include <FastLED.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <WiFi.h>

#ifdef VERSION_EPD_2_13
  #define EPD_CLASS GxEPD2_213_B74
  #define ROW1_FONT LibreFranklin_SemiBold12pt7b
  #define ROW1_ICONFONT orangeclock_icons11pt7b
  #define ROW1_ICONWIDTH 24
  #define ROW2_FONT LibreFranklin_Bold20pt7b
  #define ROW2_ICONFONT orangeclock_icons19pt7b
  #define ROW2_ICONWIDTH 38
  #define ROW3_FONT LibreFranklin_SemiBold15pt7b
  #define ROW3_ICONFONT orangeclock_icons14pt7b
  #define ROW3_ICONWIDTH 27

  #define SETUPFONT LibreFranklin_SemiBold10pt7b
#endif

#ifdef VERSION_EPD_2_9
  #define EPD_CLASS GxEPD2_290_T94
  #define ROW1_FONT LibreFranklin_SemiBold15pt7b
  #define ROW1_ICONFONT orangeclock_icons14pt7b
  #define ROW1_ICONWIDTH 29
  #define ROW2_FONT LibreFranklin_Bold25pt7b
  #define ROW2_ICONFONT orangeclock_icons25pt7b
  #define ROW2_ICONWIDTH 52
  #define ROW3_FONT LibreFranklin_SemiBold15pt7b
  #define ROW3_ICONFONT orangeclock_icons14pt7b
  #define ROW3_ICONWIDTH 29

 #define SETUPFONT LibreFranklin_SemiBold12pt7b
#endif

#define ICON_BLOCK 'A'
#define ICON_EURO 'B'
#define ICON_POUND 'C'
#define ICON_YEN 'D'
#define ICON_DOLLAR 'E'
#define ICON_PIE 'F'
#define ICON_GLOBE 'G'
#define ICON_HOURGLASS 'H'
#define ICON_LIGHTNING 'I'
#define ICON_REFRESH 'J'
#define ICON_NUCLEAR 'K'
#define ICON_SATS 'L'
#define ICON_SATUSD 'M'
#define ICON_SETTINGS 'N'
#define ICON_WIFI 'O'
#define ICON_CROSS 'P'
#define ICON_CHECK 'Q'
#define ICON_WARNING 'R'

#define SETTING_ROW1_CONTENT "row1"
#define SETTING_ROW2_CONTENT "row2"
#define SETTING_ROW3_CONTENT "row3"
#define SETTING_CURRENCY "currency"
#define SETTING_HOSTNAME_PREFIX "hostnamePrefix"
#define SETTING_MEMPOOL_INSTANCE "mempoolInstance"
#define SETTING_POWER_SAVE_MODE "powerSaveMode"
#define SETTING_TIME_OFFSET_MIN "timeOffsetMin"
#define SETTING_DECIMAL_SEPARATOR "decSeparator"
#define SETTING_TIME_FORMAT "timeFormat"
#define SETTING_DATE_FORMAT "dateFormat"


const int LINE_BLOCKHEIGHT = 0;
const int LINE_MEMPOOL_FEES = 1;
const int LINE_MEMPOOL_FEES_MEDIAN = 2;
const int LINE_HALVING_COUNTDOWN = 10;
const int LINE_SATSPERUNIT = 20;
const int LINE_FIATPRICE = 30;
const int LINE_MARKETCAP = 40;
const int LINE_TIME = 99;
const int LINE_DATE = 100;

#define CURRENCY_USD "USD"
#define CURRENCY_EUR "EUR"
#define CURRENCY_GBP "GBP"
#define CURRENCY_CAD "CAD"
#define CURRENCY_CHF "CHF"
#define CURRENCY_AUD "AUD"
#define CURRENCY_JPY "JPY"

extern WiFiClientSecure client;
extern GxEPD2_BW<EPD_CLASS, EPD_CLASS::HEIGHT> display;
extern Preferences preferences;
extern bool isUpdating;

extern String currentRow1;
extern String currentRow2;
extern String currentRow3;

extern char currentIcon1;
extern char currentIcon2;
extern char currentIcon3;

#ifdef NUM_LEDS
extern CRGB leds[NUM_LEDS];
#endif
extern volatile bool buttonPressed;