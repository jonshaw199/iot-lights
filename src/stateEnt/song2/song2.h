#ifndef STATEENT_SONG2_SONG2_H_
#define STATEENT_SONG2_SONG2_H_

#include <FastLED.h>

#include "stateEnt/virtual/lightShowBase/lightShowBase.h"

class Song2 : public LightShowBase
{
  // Stripes
  static void setupOrangeAndPurplePalette(); // Not really purple and green; to do
  static void fillFromPalette(uint8_t i);
  static void setupStripes();
  // Fire
  static void setupFire();

protected:
  static CRGB ledsA[CNT_A];
  static CRGB ledsB[CNT_B];

public:
  void preStateChange(int s);
  String getName();
  void setup();
  static void set();
  static void setValue(uint8_t v);
  static void setHue(uint8_t h);
  static void setSaturation(uint8_t s);
  bool doScanForPeersESPNow();
};

#endif