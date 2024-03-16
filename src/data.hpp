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