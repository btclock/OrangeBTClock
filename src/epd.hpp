#pragma once

#include "shared.hpp"
#include "fonts/fonts.hpp"

void setupDisplay();

void showSetupText(String t);
void updateRow1(String c);
void updateRow2(String c);
void updateRow3(String c);
void updateRows(String row1Content, String row2Content, String row3Content);