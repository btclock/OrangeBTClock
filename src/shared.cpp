#include "shared.hpp"

String getMyHostname()
{
  uint8_t mac[6];
  // WiFi.macAddress(mac);
  esp_efuse_mac_get_default(mac);
  char hostname[15];
  String hostnamePrefix = preferences.getString(SETTING_HOSTNAME_PREFIX);
  snprintf(hostname, sizeof(hostname), "%s-%02x%02x%02x", hostnamePrefix,
           mac[3], mac[4], mac[5]);
  return hostname;
}