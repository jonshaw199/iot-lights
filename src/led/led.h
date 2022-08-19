
#ifndef LED_LED_H_
#define LED_LED_H_

#ifndef CNT_A
#define CNT_A 3 // Test strips
#endif
#ifndef CNT_B
#define CNT_B 3
#endif

#include <FastLED.h>

#include <framework.h>

class LED
{
  LED();
  CRGB ledsA[CNT_A];
  CRGB ledsB[CNT_B];
  uint8_t brightness = 0;
  const uint8_t MAX_BRIGHTNESS = 255;

public:
  // LED(); // Now singleton
  static LED &getInstance();
  static void init();
  static void fillColor(CRGB color);
  static CRGB getRandColor();
  static const uint8_t getBrightness();
  static void setBrightness(uint8_t b);
};

#endif // LED_LED_H_