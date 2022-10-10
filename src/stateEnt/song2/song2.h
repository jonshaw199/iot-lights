#ifndef STATEENT_SONG2_SONG2_H_
#define STATEENT_SONG2_SONG2_H_

#include <FastLED.h>

#include "stateEnt/virtual/base/base.h"

class Song2 : public Base
{
  static void fillGrad();
  void setupPurpleAndGreenPalette();
  static void fillFromPalette(uint8_t i);

protected:
  static CRGB ledsA[CNT_A];
  static CRGB ledsB[CNT_B];

public:
  Song2();
  void preStateChange(int s);
  String getName();
  static void setBrightness(uint8_t b);
  bool doScanForPeersESPNow();
};

#endif