#ifndef UTIL_WIFI_WIFIUTIL_H_
#define UTIL_WIFI_WIFIUTIL_H_

#include <Arduino.h>

class WifiUtil
{

public:
  static void prepareWifi();
  static String setSTAMode(); // Returns MAC of STA interface
  static String setAPMode();  // Returns MAC of AP interface
  static uint8_t *macStrToPtr(const String m);
  static String macPtrToStr(const uint8_t *m);
  static void assignSTAMac(uint8_t *m);
  static void assignAPMac(uint8_t *m);
  static bool broadcastAP();
};

#endif // UTIL_WIFI_WIFIUTIL_H_