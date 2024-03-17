#include "utils.hpp"

String getAPPassword()
{
  byte mac[6];
  WiFi.macAddress(mac);
  const char charset[] = "abcdefghjkmnpqrstuvwxyzABCDEFGHJKMNPQRSTUVWXYZ23456789";
  char password[9]; // 8 characters + null terminator
  snprintf(password, sizeof(password), "%c%c%c%c%c%c%c%c",
           charset[mac[0] % (sizeof(charset) - 1)],
           charset[mac[1] % (sizeof(charset) - 1)],
           charset[mac[2] % (sizeof(charset) - 1)],
           charset[mac[3] % (sizeof(charset) - 1)],
           charset[mac[4] % (sizeof(charset) - 1)],
           charset[mac[5] % (sizeof(charset) - 1)],
           charset[(mac[0] + mac[1] + mac[2] + mac[3] + mac[4] + mac[5]) % (sizeof(charset) - 1)],
           charset[(mac[0] * mac[1] * mac[2] * mac[3] * mac[4] * mac[5]) % (sizeof(charset) - 1)]);

    return password;
}

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