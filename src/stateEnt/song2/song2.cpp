#include "song2.h"
#include "stateManager/stateManager.h"

CRGB Song2::ledsA[CNT_A];
CRGB Song2::ledsB[CNT_B];

CRGBPalette16 currentPalette;
TBlendType currentBlending;

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
  setupPurpleAndGreenPalette();

  intervalEventMap["Song2"] = IntervalEvent(
      "Song2",
      50, [](IECBArg a)
      {
  /*
  fillGrad();
  */
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */
    
    fillFromPalette( startIndex);
    
  FastLED.show();
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
  // FastLED.show();
}

void Song2::fillGrad()
{

  uint8_t starthue = beatsin8(5, 24, 28);
  uint8_t endhue = beatsin8(7, 36, 40);

  if (CNT_A)
  {
    if (starthue < endhue)
    {
      fill_gradient(ledsA, CNT_A, CHSV(starthue, 255, 255), CHSV(endhue, 255, 255), FORWARD_HUES); // If we don't have this, the colour fill will flip around.
    }
    else
    {
      fill_gradient(ledsA, CNT_A, CHSV(starthue, 255, 255), CHSV(endhue, 255, 255), BACKWARD_HUES);
    }
  }

  if (CNT_B)
  {
    if (starthue < endhue)
    {
      fill_gradient(ledsB, CNT_B, CHSV(starthue, 255, 255), CHSV(endhue, 255, 255), FORWARD_HUES); // If we don't have this, the colour fill will flip around.
    }
    else
    {
      fill_gradient(ledsB, CNT_B, CHSV(starthue, 255, 255), CHSV(endhue, 255, 255), BACKWARD_HUES);
    }
  }
}

// This function sets up a palette of purple and green stripes.
void Song2::setupPurpleAndGreenPalette()
{
  CRGB purple = CHSV(HUE_PURPLE, 255, 255);
  CRGB green = CHSV(HUE_GREEN, 255, 255);
  CRGB orange = CHSV(HUE_ORANGE - 10, 255, 255);
  CRGB black = CRGB::Black;

  currentPalette = CRGBPalette16(
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
