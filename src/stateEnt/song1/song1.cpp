#include "song1.h"

CRGB Song1::ledsA[CNT_A];
CRGB Song1::ledsB[CNT_B];

Song1::Song1()
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

  intervalEventMap.insert(std::pair<String, IntervalEvent>("SONG1_BPM", IntervalEvent(0, [](IECBArg a)
                                                                                      { return true; } /*, maxCbCnt */)));
}

void Song1::setup()
{
  Base::setup();
  fill_rainbow(ledsA, CNT_A /*led count*/, 222 /*starting hue*/);
  fill_rainbow(ledsB, CNT_B /*led count*/, 222 /*starting hue*/);
  FastLED.show();
}

void Song1::preStateChange(int s)
{
  Base::preStateChange(s);
  fill_rainbow(ledsA, CNT_A /*led count*/, CRGB(0, 0, 0));
  fill_rainbow(ledsB, CNT_B /*led count*/, CRGB(0, 0, 0));
  FastLED.show();
}

String Song1::getName()
{
  return "Song1";
}

bool Song1::doScanForPeersESPNow()
{
  return false;
}

bool Song1::doConnectToWSServer()
{
  return false;
}
