#ifndef LED_LED_H_
#define LED_LED_H_

#include <FastLED.h>

#include "state/state.h"

#if JS_ID == 2
#define CNT_A LED_CNT_BACK
#define CNT_B LED_CNT_PATIO
#elif JS_ID == 3
#define CNT_A LED_CNT_KITCHEN
#define CNT_B 0
#elif JS_ID == 4
#define CNT_A LED_CNT_GARAGE_A
#define CNT_B LED_CNT_GARAGE_B
#elif JS_ID == 5
#define CNT_A LED_CNT_FRONT
#define CNT_B 0
#elif JS_ID == 10
#define CNT_A LED_CNT_TEST
#define CNT_B 0
#else
#define CNT_A 0
#define CNT_B 0
#endif

class LED
{
  LED();
  CRGB ledsA[CNT_A];
  CRGB ledsB[CNT_B];

public:
  // LED(); // Now singleton
  static LED &getInstance();
  static void init();
  static void fillColor(CRGB color);
  static CRGB getRandColor();
};

#endif // LED_LED_H_