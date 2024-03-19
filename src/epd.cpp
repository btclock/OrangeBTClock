
#include "epd.hpp"

int row1 = 0;
int row2 = 36;
int row3 = 36 + 54;

String currentRow1 = "";
String currentRow2 = "";
String currentRow3 = "";

char currentIcon1;
char currentIcon2;
char currentIcon3;

void setupDisplay()
{
  display.init(0, true);
  display.setRotation(1);
  display.clearScreen();

  display.setRotation(1);
  display.setFont(&Antonio_SemiBold20pt7b);
  display.setTextColor(GxEPD_WHITE);
  int16_t tbx, tby;
  uint16_t tbw, tbh;
  display.getTextBounds("OrangeBTClock", 0, 0, &tbx, &tby, &tbw, &tbh);
  // center the bounding box by transposition of the origin:
  uint16_t x = ((display.width() - tbw) / 2) - tbx;
  uint16_t y = ((display.height() - tbh) / 2) - tby;
  display.fillScreen(GxEPD_BLACK);
  display.setCursor(x, y);
//  display.print("OrangeBTClock");

//  display.drawImage(epd_bitmap_allArray[0], GxEPD_WHITE, 0,0 250,37);

  int xPos = (display.width() - 250) / 2;
  int yPos = (display.height()  - 37) / 2;
  display.drawBitmap(xPos,yPos, epd_bitmap_oclogo, 250, 37, GxEPD_WHITE);
  display.display(false);

  // display.fillScreen(GxEPD_WHITE);
  // display.drawLine(0, 10, display.width(), 10, GxEPD_BLACK);
  // display.drawLine(0, row2, display.width(), row2, GxEPD_BLACK);
  // display.drawLine(0, row3, display.width(), row3, GxEPD_BLACK);
  // display.display(false);

  // display.fillScreen(GxEPD_BLACK);
  // display.display(false);

  // display.fillRect(0, 0, display.width(), 8, GxEPD_BLACK);
  // display.displayWindow(0, 0, display.width(), 8);

  // display.display(true);

  // display.setRotation(1);
  // //  display.fillRect(0, row1, display.width(), 54, GxEPD_BLACK);
  // display.displayWindow(0, row1, display.width(), row2);

  // display.display(false);

  // display.fillRect(0, row2, display.width(), 54, GxEPD_BLACK);
  // display.displayWindow(0, row2, display.width(), 54);

  // display.display(true);

  // display.fillRect(0, row3, display.width(), display.height(), GxEPD_BLACK);
  // display.displayWindow(0, row3, display.width(), display.height());

  // display.display(true);
}

void updateRow2(String c, char icon)
{
  if (c.equals(currentRow2) && icon == currentIcon2)
    return;

  display.setRotation(1);
  display.setFont(&ROW2_FONT);
  display.setTextColor(GxEPD_BLACK);
  int16_t tbx, tby;
  uint16_t tbw, tbh;
  display.getTextBounds(c, 0, 0, &tbx, &tby, &tbw, &tbh);
  // center the bounding box by transposition of the origin:
  uint16_t x = ((display.width() - (tbw + ROW2_ICONWIDTH)) / 2) - tbx;
  uint16_t y = ((display.height() - tbh) / 2) - tby;

  display.setPartialWindow(0, row2, display.width(), 48);
  display.fillRect(0, row2, display.width(), 48, GxEPD_WHITE);

  display.firstPage();
  do
  {
    display.setTextColor(GxEPD_BLACK);

    display.setFont(&ROW2_ICONFONT);
    display.setCursor(x, y);
    display.print(icon);

    display.setFont(&ROW2_FONT);
    display.setCursor(x + ROW2_ICONWIDTH, y);
    display.print(c);
  } while (display.nextPage());
  //  display.display(true);

  currentRow2 = c;
  currentIcon2 = icon;
}

void updateRow3(String c, char icon)
{
  if (c.equals(currentRow3) && icon == currentIcon3)
    return;

  display.setRotation(1);
  display.setFont(&LibreFranklin_SemiBold15pt7b);
  display.setTextColor(GxEPD_WHITE);
  int16_t tbx, tby;
  uint16_t tbw, tbh;
  display.getTextBounds(c, 0, 0, &tbx, &tby, &tbw, &tbh);
  // center the bounding box by transposition of the origin:
  uint16_t x = ((display.width() - (tbw + ROW3_ICONWIDTH)) / 2) - tbx;
  uint16_t y = row3 + (((display.height() - row3) - tbh) / 2) - tby;

  display.setPartialWindow(0, row3, display.width(), 36);
  //  display.setFullWindow();
  display.fillRect(0, row3, display.width(), 36, GxEPD_WHITE);

  display.firstPage();
  do
  {
    display.setTextColor(GxEPD_BLACK);

    display.setFont(&orangeclock_icons15pt7b);
    display.setCursor(x, y);
    display.print(icon);

    display.setFont(&LibreFranklin_SemiBold15pt7b);

    display.setCursor(x + ROW3_ICONWIDTH, y);
    display.print(c);
  } while (display.nextPage());

  currentRow3 = c;
  currentIcon3 = icon;
}

void showSetupText(String t)
{
  display.setRotation(1);
  display.setFont(&SETUPFONT);
  int16_t tbx, tby;
  uint16_t tbw, tbh;
  display.getTextBounds(t, 0, 0, &tbx, &tby, &tbw, &tbh);
  // center the bounding box by transposition of the origin:
  uint16_t x = ((display.width() - (tbw)) / 2) - tbx;
  uint16_t y = ((display.height() - tbh) / 2) - tby;
  display.setFullWindow();
  display.firstPage();
  do
  {
    display.setTextColor(GxEPD_BLACK);
    display.setFont(&ROW2_ICONFONT);
    display.setCursor(0, 40);
    display.println(ICON_WIFI);

    display.setFont(&SETUPFONT);
    display.println(t);
  } while (display.nextPage());
}

void updateRow1(String c, char icon)
{
  if (c.equals(currentRow1) && icon == currentIcon1)
    return;

  // struct tm timeinfo;
  // if (!getLocalTime(&timeinfo))
  // {
  //   if (Serial.available())
  //     Serial.println("Failed to obtain time");
  //   return;
  // }

  // char dateString[10];
  // strftime(dateString, 10, "%H:%M:%S", &timeinfo);

  display.setRotation(1);
  display.setFont(&ROW1_FONT);
  display.setTextColor(GxEPD_WHITE);
  int16_t tbx, tby;
  uint16_t tbw, tbh;
  display.getTextBounds(c, 0, 0, &tbx, &tby, &tbw, &tbh);
  // center the bounding box by transposition of the origin:
  uint16_t x = ((display.width() - (tbw + ROW1_ICONWIDTH)) / 2) - tbx;
  uint16_t y = (((row2)-tbh) / 2) - tby;

  display.setPartialWindow(0, 0, display.width(), 36);
  display.fillRect(0, 0, display.width(), 36, GxEPD_WHITE);

  display.firstPage();
  do
  {
    display.setTextColor(GxEPD_BLACK);

    display.setFont(&ROW1_ICONFONT);
    display.setCursor(x, y);
    display.print(icon);

    display.setFont(&ROW1_FONT);

    display.setCursor(x + ROW1_ICONWIDTH, y);
    display.print(c);
  } while (display.nextPage());

  currentRow1 = c;
  currentIcon1 = icon;
}

void updateRows(String row1Content, String row2Content, String row3Content)
{
  display.setRotation(1);
  display.setFont(&ROW1_FONT);
  display.setTextColor(GxEPD_BLACK);
  int16_t tbx, tby;
  uint16_t tbw, tbh;
  display.getTextBounds(row1Content, 0, 0, &tbx, &tby, &tbw, &tbh);
  // center the bounding box by transposition of the origin:
  uint16_t row1_x = ((display.width() - (tbw + ROW1_ICONWIDTH)) / 2) - tbx;
  uint16_t row1_y = (((row2)-tbh) / 2) - tby;

  display.setFont(&ROW2_FONT);
  display.getTextBounds(row2Content, 0, 0, &tbx, &tby, &tbw, &tbh);
  // center the bounding box by transposition of the origin:
  uint16_t row2_x = ((display.width() - (tbw + ROW2_ICONWIDTH)) / 2) - tbx;
  uint16_t row2_y = ((display.height() - tbh) / 2) - tby;

  display.setFont(&ROW3_FONT);
  display.getTextBounds(row3Content, 0, 0, &tbx, &tby, &tbw, &tbh);
  // center the bounding box by transposition of the origin:
  uint16_t row3_x = ((display.width() - (tbw + ROW3_ICONWIDTH)) / 2) - tbx;
  uint16_t row3_y = row3 + (((display.height() - row3) - tbh) / 2) - tby;

  // if (!currentRow1.equals(row1Content) && !currentRow2.equals(row2Content) && !currentRow3.equals(row3Content))
  // {
  display.setFullWindow();
  // }
  // else
  if (currentRow1.equals(row1Content) && currentRow2.equals(row2Content) && currentRow3.equals(row3Content))
  {
    Serial.println(F("Nothing to update"));
    return;
  }

  // display.setPartialWindow(0, display.width(), 0, display.height());
  //  else {
  //   if (!currentRow1.equals(row1Content))
  //   {
  //     display.setPartialWindow(0, 0, display.width(), 36);
  //     Serial.print(F("Row 1 to: "));
  //     Serial.println(row1Content);
  //   }

  //   if (!currentRow2.equals(row2Content))
  //   {
  //     display.setPartialWindow(0, row2, display.width(), 56);
  //     Serial.print(F("Row 2 to: "));
  //     Serial.println(row2Content);
  //   }

  //   if (!currentRow3.equals(row3Content))
  //   {
  //     display.setPartialWindow(0, row3, display.width(), 36);
  //     Serial.print(F("Row 3 to: "));
  //     Serial.println(row3Content);
  //   }
  // }

  display.firstPage();
  do
  {
    display.setTextColor(GxEPD_BLACK);

    if (!currentRow1.equals(row1Content))
    {

      display.fillRect(0, 0, display.width(), 36, GxEPD_WHITE);

      display.setFont(&ROW1_ICONFONT);
      display.setCursor(row1_x, row1_y);
      display.print(ICON_PIE);

      display.setFont(&ROW1_FONT);
      display.setCursor(row1_x + ROW1_ICONWIDTH, row1_y);
      display.print(row1Content);

      currentRow1 = row1Content;
    }

    if (!currentRow2.equals(row2Content))
    {
      display.fillRect(0, row2, display.width(), 54, GxEPD_WHITE);
      display.setTextColor(GxEPD_BLACK);

      display.setFont(&ROW2_ICONFONT);
      display.setCursor(row2_x, row2_y);
      display.print(ICON_BLOCK);

      display.setFont(&ROW2_FONT);
      display.setCursor(row2_x + ROW2_ICONWIDTH, row2_y);
      display.print(row2Content);

      currentRow2 = row2Content;
    }

    if (!currentRow3.equals(row3Content))
    {
      display.fillRect(0, row3, display.width(), 36, GxEPD_WHITE);
      display.setFont(&orangeclock_icons15pt7b);
      display.setCursor(row3_x, row3_y);
      display.print(ICON_DOLLAR);

      display.setFont(&LibreFranklin_SemiBold15pt7b);
      display.setCursor(row3_x + ROW3_ICONWIDTH, row3_y);
      display.print(row3Content);

      currentRow3 = row3Content;
    }
  } while (display.nextPage());
}