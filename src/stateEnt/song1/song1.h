#ifndef STATEENT_SONG1_SONG1_H_
#define STATEENT_SONG1_SONG1_H_

#include <FastLED.h>

#include "stateEnt/virtual/base/base.h"

class Song1 : public Base
{
protected:
  static CRGB ledsA[CNT_A];
  static CRGB ledsB[CNT_B];

public:
  Song1();
  void preStateChange(int s);
  String getName();
  void doSynced();
  bool doSync();
  static void setBrightness(uint8_t b);
};

#endif