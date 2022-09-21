
#include "led.h"

JSLED::JSLED()
{
}

JSLED &JSLED::getInstance()
{
  static JSLED instance; // Guaranteed to be destroyed.
                         // Instantiated on first use.
  return instance;
}

void JSLED::init()
{
  if (CNT_A)
  {
#if LED_2811
    FastLED.addLeds<WS2811, LED_PIN_A>(getInstance().ledsA, CNT_A);
#else
    FastLED.addLeds<WS2812, LED_PIN_A, GRB>(getInstance().ledsA, CNT_A);
#endif
  }
  if (CNT_B)
  {
#if LED_2811
    FastLED.addLeds<WS2811, LED_PIN_B>(getInstance().ledsB, CNT_B);
#else
    FastLED.addLeds<WS2812, LED_PIN_B, GRB>(getInstance().ledsB, CNT_B);
#endif
  }
  FastLED.setBrightness(200);
}

void JSLED::fillColor(CRGB color)
{
  if (CNT_A)
  {
    fill_solid(getInstance().ledsA, CNT_A, color);
  }
  if (CNT_B)
  {
    fill_solid(getInstance().ledsB, CNT_B, color);
  }
  FastLED.show();
}

CRGB JSLED::getRandColor()
{
  CRGB options[] = {CRGB::Red, CRGB::Blue, CRGB::Green, CRGB::White};
  int i = rand() % 4;
  return options[i];
}

const uint8_t JSLED::getBrightness()
{
  return getInstance().brightness;
}

void JSLED::setBrightness(uint8_t b)
{
  getInstance().brightness = b;
  FastLED.setBrightness(b);
}
