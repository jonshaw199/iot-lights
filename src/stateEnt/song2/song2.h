#ifndef STATEENT_SONG2_SONG2_H_
#define STATEENT_SONG2_SONG2_H_

#include <FastLED.h>

#include "stateEnt/virtual/lightShowBase/lightShowBase.h"

class Song2 : public LightShowBase
{
  // Shared
  static void setupHalloweenPalette();
  // Stripes
  static void fillFromPalette(uint8_t i);
  static void setupStripes();
  // Fire
  static void setupFire();
  // Noise
  static void setupNoise();
  static void fillNoise8();
  static void setTargetPalette(unsigned int seed = 0);
  // Breathing
  static void setupBreathing();
  static void breath();
  // Lightning
  static void setupLightning();

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