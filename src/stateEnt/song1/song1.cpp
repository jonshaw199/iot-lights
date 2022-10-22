#if CNT_A || CNT_B

#include "song1.h"
#include "stateManager/stateManager.h"

CRGB Song1::ledsA[CNT_A];
CRGB Song1::ledsB[CNT_B];

Song1::Song1()
{
  if (CNT_A)
  {
    FastLED.addLeds<LED_TYPE_A, LED_PIN_A, LED_ORDER_A>(ledsA, CNT_A);
  }
  if (CNT_B)
  {
    FastLED.addLeds<LED_TYPE_B, LED_PIN_B, LED_ORDER_B>(ledsB, CNT_B);
  }
  FastLED.setBrightness(150);
  // FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
}

void Song1::preStateChange(int s)
{
  LightShowBase::preStateChange(s);
  // Turn off lights
  if (CNT_A)
  {
    fill_solid(ledsA, CNT_A, CRGB::Black);
  }
  if (CNT_B)
  {
    fill_solid(ledsB, CNT_B, CRGB::Black);
  }

  FastLED.show();
}

String Song1::getName()
{
  return "Song1";
}

void Song1::doSynced()
{
  AF1::setIE(IntervalEvent(
      "Song1",
      15, [](IECBArg a)
      {
  uint8_t beatA = beatsin8(17, 0, 255);                        // Starting hue
  uint8_t beatB = beatsin8(13, 0, 255);
  if (CNT_A) {
fill_rainbow(ledsA, CNT_A, (beatA+beatB)/2, 8); 
  }
  if (CNT_B) {
    fill_rainbow(ledsB, CNT_B, (beatA+beatB)/2, 8); 
  }
  FastLED.show(); },
      -1, true));
}

bool Song1::doSync()
{
  return true;
}

void Song1::setBrightness(uint8_t b)
{
  Serial.print("Setting brightness to ");
  Serial.println(b);
  FastLED.setBrightness(b);
  // FastLED.show();
}

#endif