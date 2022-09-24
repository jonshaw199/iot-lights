#include "show.h"

CRGB Show::ledsA[CNT_A];
CRGB Show::ledsB[CNT_B];

Show::Show()
{
  if (CNT_A)
  {
#if LED_2811
    FastLED.addLeds<WS2811, LED_PIN_A>(ledsA, CNT_A);
#else
    FastLED.addLeds<WS2812, LED_PIN_A, GRB>(ledsA, CNT_A);
#endif
  }
  if (CNT_B)
  {
#if LED_2811
    FastLED.addLeds<WS2811, LED_PIN_B>(ledsB, CNT_B);
#else
    FastLED.addLeds<WS2812, LED_PIN_B, GRB>(ledsB, CNT_B);
#endif
  }
}

void Show::setup()
{
  fill_rainbow(ledsA, CNT_A /*led count*/, 222 /*starting hue*/);
  fill_rainbow(ledsB, CNT_B /*led count*/, 222 /*starting hue*/);
  FastLED.show();
}

void Show::preStateChange(int s)
{
  fill_rainbow(ledsA, CNT_A /*led count*/, CRGB(0, 0, 0));
  fill_rainbow(ledsB, CNT_B /*led count*/, CRGB(0, 0, 0));
  FastLED.show();
}

String Show::getName()
{
  return "Show";
}

bool Show::doScanForPeersESPNow()
{
  return false;
}

bool Show::doConnectToWSServer()
{
  return false;
}
