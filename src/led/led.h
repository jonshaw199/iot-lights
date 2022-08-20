
#ifndef LED_LED_H_
#define LED_LED_H_

#ifdef LED_PIN_A
#ifndef CNT_A
#define CNT_A 3 // Test strips
#endif
#endif
#ifdef LED_PIN_B
#ifndef CNT_B
#define CNT_B 3
#endif
#endif

#include <FastLED.h>

#include <framework.h>

class JSLED
{
  JSLED();
  CRGB ledsA[CNT_A];
  CRGB ledsB[CNT_B];
  uint8_t brightness = 0;
  const uint8_t MAX_BRIGHTNESS = 255;

public:
  // LED(); // Now singleton
  static JSLED &getInstance();
  static void init();
  static void fillColor(CRGB color);
  static CRGB getRandColor();
  static const uint8_t getBrightness();
  static void setBrightness(uint8_t b);
};

#endif // LED_LED_H_