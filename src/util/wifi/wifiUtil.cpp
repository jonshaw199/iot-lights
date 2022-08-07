
#include <WiFi.h>

#include "wifiUtil.h"

void WifiUtil::prepareWifi()
{
  WiFi.disconnect(true);
  delay(DELAY_PREPARE_WIFI);
}

void WifiUtil::setSTAMode()
{
  prepareWifi();
  Serial.println("Setting wifi mode to STA");
  WiFi.mode(WIFI_STA);
}

void WifiUtil::setAPMode()
{
  prepareWifi();
  Serial.println("Setting wifi mode to AP");
  WiFi.mode(WIFI_AP);
}

// Setup access point (aka open wifi network); this is used by slaves so master can find them
bool WifiUtil::broadcastAP()
{
  prepareWifi();
  Serial.println("Broadcasting soft AP");
  String Prefix = DEVICE_PREFIX;
  String id = String(JS_ID);
  String SSID = Prefix + id;
  String Password = DEVICE_AP_PASS;
  return WiFi.softAP(SSID.c_str(), Password.c_str(), ESPNOW_CHANNEL, 0);
}

void WifiUtil::printMac(const uint8_t *m)
{
  Serial.print("{");
  for (int i = 0; i < 6; i++)
  {
    Serial.print("0x");
    Serial.print(m[i], HEX);
    if (i < 5)
      Serial.print(',');
  }
  Serial.println("}");
}
