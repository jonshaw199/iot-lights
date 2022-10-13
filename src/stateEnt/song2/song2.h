#ifndef STATEENT_SONG2_SONG2_H_
#define STATEENT_SONG2_SONG2_H_

#include <FastLED.h>

#include "stateEnt/virtual/lightShowBase/lightShowBase.h"

class Song2 : public LightShowBase
{
  void setupPurpleAndGreenPalette();
  static void fillFromPalette(uint8_t i);

protected:
  static CRGB ledsA[CNT_A];
  static CRGB ledsB[CNT_B];

public:
  Song2();
  void preStateChange(int s);
  String getName();
  static void set();
  static void setValue(uint8_t v);
  static void setHue(uint8_t h);
  static void setSaturation(uint8_t s);
  bool doScanForPeersESPNow();
};

#endif