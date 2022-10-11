#include "song2.h"
#include "stateManager/stateManager.h"

CRGB Song2::ledsA[CNT_A];
CRGB Song2::ledsB[CNT_B];

CRGBPalette16 currentPalette;
TBlendType currentBlending;

bool override = false;

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
  // currentBlending = LINEARBLEND;
  currentBlending = NOBLEND;
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
  Base::preStateChange(s);
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

void Song2::setBrightness(uint8_t b)
{
  Serial.print("Setting brightness to ");
  Serial.println(b);
  FastLED.setBrightness(b);
  FastLED.show();
}

void Song2::setHue(uint8_t h)
{
  Serial.print("Setting hue to ");
  Serial.println(h);
  override = true;
  if (CNT_A)
  {
    fill_solid(ledsA, CNT_A, h);
  }
  if (CNT_B)
  {
    fill_solid(ledsB, CNT_B, h);
  }
  FastLED.show();
}

// This function sets up a palette of purple and green stripes.
void Song2::setupPurpleAndGreenPalette()
{
  CHSV purple = CHSV(HUE_PURPLE, 255, 255);
  CHSV green = CHSV(HUE_GREEN, 255, 255);
  CHSV orange = CHSV(HUE_ORANGE, 255, 255);

  currentPalette = CHSVPalette16(
      orange, purple, orange, purple,
      orange, purple, orange, purple,
      orange, purple, orange, purple,
      orange, purple, orange, purple);
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
