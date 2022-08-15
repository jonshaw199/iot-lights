
#ifndef LED_LED_H_
#define LED_LED_H_

#if JS_ID == 2
#define CNT_A 287
#define CNT_B 378
#elif JS_ID == 3
#define CNT_A 150
#define CNT_B 0
#elif JS_ID == 4
#define CNT_A 409
#define CNT_B 350
#elif JS_ID == 5
#define CNT_A 333
#define CNT_B 0
#else
#define CNT_A 3
#define CNT_B 0
#endif

#include <FastLED.h>

#include "state/state.h"

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