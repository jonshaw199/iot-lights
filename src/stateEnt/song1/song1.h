#ifndef STATEENT_SONG1_SONG1_H_
#define STATEENT_SONG1_SONG1_H_

#include <AF1.h>
#include <FastLED.h>

class Song1 : public Base
{
protected:
  static CRGB ledsA[CNT_A];
  static CRGB ledsB[CNT_B];
  static int hue;

public:
  Song1();
  void setup();
  void preStateChange(int s);
  String getName();
  bool doScanForPeersESPNow();
  bool doConnectToWSServer();
};

#endif