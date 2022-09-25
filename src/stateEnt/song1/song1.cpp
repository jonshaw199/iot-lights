#include "song1.h"

CRGB Song1::ledsA[CNT_A];
CRGB Song1::ledsB[CNT_B];
int Song1::hue = 222;

Song1::Song1()
{
  FastLED.addLeds<LED_TYPE_A, LED_PIN_A, LED_ORDER_A>(ledsA, CNT_A);
  FastLED.addLeds<LED_TYPE_B, LED_PIN_B, LED_ORDER_B>(ledsB, CNT_B);
  intervalEventMap.insert(std::pair<String, IntervalEvent>("SONG1_1", IntervalEvent(50, [](IECBArg a)
                                                                                    {
  hue = (hue + 5) % 255; 
  fill_rainbow(ledsA, CNT_A /*led count*/, hue /*starting hue*/);
  fill_rainbow(ledsB, CNT_B /*led count*/, hue /*starting hue*/);
  FastLED.show();
  return true; } /*, maxCbCnt */)));
}

void Song1::setup()
{
  Base::setup();
  fill_rainbow(ledsA, CNT_A /*led count*/, hue /*starting hue*/);
  fill_rainbow(ledsB, CNT_B /*led count*/, hue /*starting hue*/);
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
