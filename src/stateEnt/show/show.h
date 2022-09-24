#ifndef STATEENT_SHOW_SHOW_H_
#define STATEENT_SHOW_SHOW_H_

#include <AF1.h>
#include <FastLED.h>

#ifndef CNT_A
#ifdef LED_PIN_A
#define CNT_A 3 // Test strips
#else
#define CNT_A 0
#define LED_PIN_A 0
#endif
#endif

#ifndef CNT_B
#ifdef LED_PIN_B
#define CNT_B 3
#else
#define CNT_B 0
#define LED_PIN_B 0
#endif
#endif

class Show : public Base
{
protected:
  static CRGB ledsA[CNT_A];
  static CRGB ledsB[CNT_B];

public:
  Show();
  void setup();
  void preStateChange(int s);
  String getName();
  bool doScanForPeersESPNow();
  bool doConnectToWSServer();
};

#endif