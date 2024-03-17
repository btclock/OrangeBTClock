#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <pgmspace.h>
#include <cstring>

#include "config.hpp"
#include "shared.hpp"

uint getPrice();
uint getBlock();
String getMempoolFees();
uint getMempoolFeesMedian();
double getSupplyAtBlock(std::uint32_t blockNr);
String formatNumberWithSuffix(std::uint64_t num, int numCharacters);