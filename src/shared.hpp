#pragma once

#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <Preferences.h>
#include <GxEPD2.h>
#include <GxEPD2_BW.h>
#include "fonts/fonts.hpp"

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
  #define ROW1_ICONWIDTH 27
  #define ROW2_FONT LibreFranklin_Bold25pt7b
  #define ROW2_ICONFONT orangeclock_icons25pt7b
  #define ROW2_ICONWIDTH 52
  #define ROW3_FONT LibreFranklin_SemiBold15pt7b
  #define ROW3_ICONFONT orangeclock_icons14pt7b
  #define ROW3_ICONWIDTH 27

 #define SETUPFONT LibreFranklin_SemiBold12pt7b
#endif

#define ICON_BLOCK "A"
#define ICON_EURO "B"
#define ICON_POUND "C"
#define ICON_YEN "D"
#define ICON_DOLLAR "E"
#define ICON_PIE "F"
#define ICON_GLOBE "G"
#define ICON_HOURGLASS "H"
#define ICON_LIGHTNING "I"
#define ICON_REFRESH "J"
#define ICON_NUCLEAR "K"
#define ICON_SATS "L"
#define ICON_SATUSD "M"
#define ICON_SETTINGS "N"
#define ICON_WIFI "O"
#define ICON_CROSS "P"
#define ICON_CHECK "Q"
#define ICON_WARNING "R"

const int LINE_BLOCKHEIGHT = 0;
const int LINE_HALVING_COUNTDOWN = 1;
const int LINE_SATSPERDOLLAR = 2;
const int LINE_FIATPRICE = 3;
const int LINE_MEMPOOL_FEES = 4;
const int LINE_TIME = 5;
const int LINE_DATE = 6;

extern WiFiClientSecure client;
extern GxEPD2_BW<EPD_CLASS, EPD_CLASS::HEIGHT> display;
extern Preferences preferences;
extern bool isUpdating;