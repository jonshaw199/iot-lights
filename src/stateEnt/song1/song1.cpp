#include "song1.h"

CRGB Song1::ledsA[CNT_A];
CRGB Song1::ledsB[CNT_B];

Song1::Song1()
{
  FastLED.addLeds<LED_TYPE_A, LED_PIN_A, LED_ORDER_A>(ledsA, CNT_A);
  FastLED.addLeds<LED_TYPE_B, LED_PIN_B, LED_ORDER_B>(ledsB, CNT_B);
  FastLED.setBrightness(150);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);

  setSyncedTask([](STArg a)
                { StateManager::getCurStateEnt()->getIntervalEventMap()["Song1"] = IntervalEvent(
                      "Song1",
                      1, [](IECBArg a)
                      {
  uint8_t beatA = beatsin8(17, 0, 255);                        // Starting hue
  uint8_t beatB = beatsin8(13, 0, 255);
  fill_rainbow(ledsA, CNT_A, (beatA+beatB)/2, 8); 
  fill_rainbow(ledsB, CNT_B, (beatA+beatB)/2, 8); 
  return true; },
                      -1, true); });
}

void Song1::preStateChange(int s)
{
  Song::preStateChange(s);
  // Turn off lights
  fill_solid(ledsA, CNT_A, CRGB::Black);
  fill_solid(ledsB, CNT_B, CRGB::Black);
  FastLED.show();
}

String Song1::getName()
{
  return "Song1";
}
