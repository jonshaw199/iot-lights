#ifndef UTIL_WIFI_WIFIUTIL_H_
#define UTIL_WIFI_WIFIUTIL_H_

#include <Arduino.h>

class WifiUtil
{

public:
  static void prepareWifi();
  static void setSTAMode();
  static void setAPMode();
  static bool broadcastAP();
  static void printMac(const uint8_t *m);
};

#endif // UTIL_WIFI_WIFIUTIL_H_