#ifndef STATEENT_SONG2_SONG2_H_
#define STATEENT_SONG2_SONG2_H_

#include <FastLED.h>

#include "stateEnt/virtual/song/song.h"

class Song2 : public Song
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
  // Discostrobe Halloween
  static void setupDisco();
  static void discostrobe();
  static void discoWorker(
      uint8_t dashperiod, uint8_t dashwidth, int8_t dashmotionspeed,
      uint8_t stroberepeats,
      uint8_t huedelta);
  static void drawRainbowDashes(
      uint8_t startpos, uint16_t lastpos, uint8_t period, uint8_t width,
      uint8_t huestart, uint8_t huedelta, uint8_t saturation, uint8_t value);
  // Twinkefox
  static void setupTwinklefox();
  static void chooseNextColorPalette(CRGBPalette16 &pal);
  static void drawTwinkles(CRGBSet &L);
  static CRGB computeOneTwinkle(uint32_t ms, uint8_t salt);
  static uint8_t attackDecayWave8(uint8_t i);
  static void coolLikeIncandescent(CRGB &c, uint8_t phase);

public:
  void preStateChange(int s);
  String getName();
  void setup();
  static void setHSV();
  static void setValue(uint8_t v);
  static void setHue(uint8_t h);
  static void setSaturation(uint8_t s);
  bool doScanForPeersESPNow();
};

#endif