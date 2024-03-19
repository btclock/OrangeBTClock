#pragma once

#include "shared.hpp"
#include "fonts/fonts.hpp"
#include "bitmap.hpp"
void setupDisplay();

void showSetupText(String t);
void updateRow1(String c, char icon);
void updateRow2(String c, char icon);
void updateRow3(String c, char icon);
void updateRows(String row1Content, String row2Content, String row3Content);