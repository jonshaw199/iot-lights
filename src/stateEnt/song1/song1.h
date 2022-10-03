#ifndef STATEENT_SONG1_SONG1_H_
#define STATEENT_SONG1_SONG1_H_

#include <FastLED.h>

#include "stateEnt/virtual/song/song.h"

class Song1 : public Song
{
protected:
  static CRGB ledsA[CNT_A];
  static CRGB ledsB[CNT_B];

public:
  Song1();
  void preStateChange(int s);
  String getName();
  void doSynced(STArg a);
};

#endif