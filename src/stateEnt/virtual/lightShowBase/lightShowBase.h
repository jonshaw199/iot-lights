#ifndef STATEENT_VIRTUAL_LIGHTSHOWBASE_LIGHTSHOWBASE_H_
#define STATEENT_VIRTUAL_LIGHTSHOWBASE_LIGHTSHOWBASE_H_

#include <AF1.h>
#include <FastLED.h>

#define max(a, b) ((a) >= (b) ? (a) : (b))
#define CNT max(CNT_A, CNT_B)

class LightShowBase : public Base
{
protected:
  static int ledCnt;
  // CRGB leds[CNT];
  static CRGBArray<CNT> leds;

public:
  void setup();
  void loop();
  bool doScanForPeersESPNow();
  void onConnectWSServer();
  DynamicJsonDocument getInfo();
  msg_handler getInboxHandler();
  void preStateChange(int s);
#ifdef VS1053_CS_PIN
  static void pausePlaying(bool p);
  static void stopPlaying();
  static bool playFile(String f);
#endif
};

#endif