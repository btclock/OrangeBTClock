#pragma once

#include <WiFi.h>
#include <Arduino.h>
#include <ArduinoJson.hpp>
#include "shared.hpp"

namespace ArduinoJson {
template <>
struct Converter<char> {
  static void toJson(char c, JsonVariant var) {
    var.set(static_cast<signed char>(c));
  }

  static char fromJson(JsonVariantConst src) {
    return static_cast<char>(src.as<signed char>());
  }

  static bool checkJson(JsonVariantConst src) {
    return src.is<signed char>();
  }
};
}


String getAPPassword();
String getMyHostname();
