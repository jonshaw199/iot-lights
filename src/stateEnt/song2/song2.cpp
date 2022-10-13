#include "song2.h"
#include "stateManager/stateManager.h"

CRGB Song2::ledsA[CNT_A];
CRGB Song2::ledsB[CNT_B];

CRGBPalette16 currentPalette;
TBlendType currentBlending;

bool override = false;

uint8_t hue = 50;
uint8_t saturation = 255;
uint8_t value = 255;

Song2::Song2()
{
  if (CNT_A)
  {
    FastLED.addLeds<LED_TYPE_A, LED_PIN_A, LED_ORDER_A>(ledsA, CNT_A);
  }
  if (CNT_B)
  {
    FastLED.addLeds<LED_TYPE_B, LED_PIN_B, LED_ORDER_B>(ledsB, CNT_B);
  }
  FastLED.setBrightness(200);
  // FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);

  // currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
  // currentBlending = NOBLEND;
  setupPurpleAndGreenPalette();

  intervalEventMap["Song2"] = IntervalEvent(
      "Song2",
      25, [](IECBArg a)
      {
        if (!override) {
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */
    
    fillFromPalette( startIndex);
    
  FastLED.show();
        }
  return true; });
}

void Song2::preStateChange(int s)
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

String Song2::getName()
{
  return "Song2";
}

void Song2::set()
{
  override = true;
  if (CNT_A)
  {
    fill_solid(ledsA, CNT_A, CHSV(hue, saturation, value));
  }
  if (CNT_B)
  {
    fill_solid(ledsB, CNT_B, CHSV(hue, saturation, value));
  }
  FastLED.show();
}

void Song2::setValue(uint8_t v)
{
  value = v;
  set();
}

void Song2::setHue(uint8_t h)
{
  hue = h;
  set();
}

void Song2::setSaturation(uint8_t s)
{
  saturation = s;
  set();
}

// This function sets up a palette of purple and green stripes.
void Song2::setupPurpleAndGreenPalette()
{
  CHSV purple = CHSV(213, 255, 50);
  CHSV orange = CHSV(5, 255, 150);

  currentPalette = CHSVPalette16(
      orange, orange, orange, orange,
      purple, purple, purple, purple,
      orange, orange, orange, orange,
      purple, purple, purple, purple);
}

void Song2::fillFromPalette(uint8_t colorIndex)
{
  uint8_t brightness = 255;

  for (int i = 0; i < CNT_A; ++i)
  {
    ledsA[i] = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending);
    colorIndex += 3;
  }

  for (int i = 0; i < CNT_B; ++i)
  {
    ledsB[i] = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending);
    colorIndex += 3;
  }
}

bool Song2::doScanForPeersESPNow()
{
  return false;
}
