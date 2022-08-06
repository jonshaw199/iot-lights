
#include <WiFi.h>

#include "wifiUtil.h"

void WifiUtil::prepareWifi()
{
  WiFi.disconnect(true);
  delay(DELAY_PREPARE_WIFI);
}

String WifiUtil::setSTAMode()
{
  prepareWifi();
  WiFi.mode(WIFI_STA);
  return WiFi.macAddress();
}

String WifiUtil::setAPMode()
{
  prepareWifi();
  WiFi.mode(WIFI_AP);
  return WiFi.softAPmacAddress();
}

uint8_t *WifiUtil::macStrToPtr(const String m)
{
  uint8_t *mac = new uint8_t[6];
  sscanf(m.c_str(), "%x:%x:%x:%x:%x:%x", &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);
  return mac;
}

String WifiUtil::macPtrToStr(const uint8_t *m)
{
  String mac = "";
  for (int i = 0; i < 6; i++)
  {
    mac += m[i];
  }
  return mac;
}

void WifiUtil::assignSTAMac(uint8_t *m)
{
  uint8_t *mac = macStrToPtr(setSTAMode());
  memcpy(m, mac, 6);
}

void WifiUtil::assignAPMac(uint8_t *m)
{
  uint8_t *mac = macStrToPtr(setAPMode());
  memcpy(m, mac, 6);
}

// Setup access point (aka open wifi network); this is used by slaves so master can find them
bool WifiUtil::broadcastAP()
{
  String Prefix = DEVICE_PREFIX;
  String id = String(JS_ID);
  String SSID = Prefix + id;
  String Password = DEVICE_AP_PASS;
  return WiFi.softAP(SSID.c_str(), Password.c_str(), ESPNOW_CHANNEL, 0);
}