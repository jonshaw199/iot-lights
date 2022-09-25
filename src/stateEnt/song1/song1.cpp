#include "song1.h"

CRGB Song1::ledsA[CNT_A];
CRGB Song1::ledsB[CNT_B];

Song1::Song1()
{
  FastLED.addLeds<LED_TYPE_A, LED_PIN_A, LED_ORDER_A>(ledsA, CNT_A);
  FastLED.addLeds<LED_TYPE_B, LED_PIN_B, LED_ORDER_B>(ledsB, CNT_B);
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
  fill_solid(ledsA, CNT_A, CRGB::Black);
  fill_solid(ledsB, CNT_B, CRGB::Black);
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
