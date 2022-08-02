#ifndef SLAVE_LED_LED_H_
#define SLAVE_LED_LED_H_

#include <FastLED.h>

#include "base/base.h"
#include "state/state.h"

class LED : public Base
{
  CRGB ledsPatio[LED_CNT_PATIO];
  CRGB ledsBack[LED_CNT_BACK];
  CRGB ledsKitchen[LED_CNT_KITCHEN];
  CRGB ledsGarageA[LED_CNT_GARAGE_A];
  CRGB ledsGarageB[LED_CNT_GARAGE_B];
  CRGB ledsFront[LED_CNT_FRONT];
  CRGB *ledsA;
  CRGB *ledsB;
  int ledsCntA;
  int ledsCntB;

public:
  void setup();
  void loop();
  bool preStateChange(JSState s);
  void fillColor(CRGB color);
  static CRGB getRandColor();
};

#endif // SLAVE_LED_LED_H_