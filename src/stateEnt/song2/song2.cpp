#include "song2.h"
#include "stateManager/stateManager.h"

CRGB Song2::ledsA[CNT_A];
CRGB Song2::ledsB[CNT_B];

CRGBPalette16 currentPalette;
TBlendType currentBlending;

uint8_t hue = 50;
uint8_t saturation = 255;
uint8_t value = 255;

int dirCoef = 1;

void Song2::setup()
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
  // setupStripes();
  setupFire();
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

bool Song2::doScanForPeersESPNow()
{
  return false;
}

void Song2::set()
{
  StateManager::getCurStateEnt()->deactivateIntervalEvents();
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
void Song2::setupOrangeAndPurplePalette()
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

void Song2::setupStripes()
{
  // currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
  // currentBlending = NOBLEND;
  setupOrangeAndPurplePalette();

  StateManager::getCurStateEnt()->getIntervalEventMap()["Song2"] = IntervalEvent(
      "Song2",
      25, [](IECBArg a)
      {
    static uint8_t startIndex = 0;
    startIndex = startIndex + dirCoef; /* motion speed... and direction */
    
    fillFromPalette( startIndex);
    
  FastLED.show();
  return true; });

  StateManager::getCurStateEnt()->getIntervalEventMap()["Song2_Direction"] = IntervalEvent(
      "Song2_Direction",
      15000, [](IECBArg a)
      {
        dirCoef = dirCoef * -1;
  return true; });
}

// Fire2012 by Mark Kriegsman, July 2012
// as part of "Five Elements" shown here: http://youtu.be/knWiGsmgycY
////
// This basic one-dimensional 'fire' simulation works roughly as follows:
// There's a underlying array of 'heat' cells, that model the temperature
// at each point along the line.  Every cycle through the simulation,
// four steps are performed:
//  1) All cells cool down a little bit, losing heat to the air
//  2) The heat from each cell drifts 'up' and diffuses a little
//  3) Sometimes randomly new 'sparks' of heat are added at the bottom
//  4) The heat from each cell is rendered as a color into the leds array
//     The heat-to-color mapping uses a black-body radiation approximation.
//
// Temperature is in arbitrary units from 0 (cold black) to 255 (white hot).
//
// This simulation scales it self a bit depending on NUM_LEDS; it should look
// "OK" on anywhere from 20 to 100 LEDs without too much tweaking.
//
// I recommend running this simulation at anywhere from 30-100 frames per second,
// meaning an interframe delay of about 10-35 milliseconds.
//
// Looks best on a high-density LED setup (60+ pixels/meter).
//
//
// There are two main parameters you can play with to control the look and
// feel of your fire: COOLING (used in step 1 above), and SPARKING (used
// in step 3 above).
//
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 50, suggested range 20-100
#define COOLING 55

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 120

void Song2::setupFire()
{
  StateManager::getCurStateEnt()->getIntervalEventMap()["Song2"] = IntervalEvent(
      "Song2",
      25, [](IECBArg a)
      {
  static bool gReverseDirection = false;

  if (CNT_A) {
  // Array of temperature readings at each simulation cell
    static uint8_t heatA[CNT_A];

    // Step 1.  Cool down every cell a little
    for (int i = 0; i < CNT_A; i++)
    {
      heatA[i] = qsub8(heatA[i], random8(0, ((COOLING * 10) / CNT_A) + 2));
    }

    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for (int k = CNT_A - 1; k >= 2; k--)
    {
      heatA[k] = (heatA[k - 1] + heatA[k - 2] + heatA[k - 2]) / 3;
    }

    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if (random8() < SPARKING)
    {
      int y = random8(7);
      heatA[y] = qadd8(heatA[y], random8(160, 255));
    }

    // Step 4.  Map from heat cells to LED colors
    for (int j = 0; j < CNT_A; j++)
    {
      CRGB color = HeatColor(heatA[j]);
      int pixelnumber;
      if (gReverseDirection)
      {
        pixelnumber = (CNT_A - 1) - j;
      }
      else
      {
        pixelnumber = j;
      }
      ledsA[pixelnumber] = color;
    }
  }

  if (CNT_B) {
  // Array of temperature readings at each simulation cell
    static uint8_t heatB[CNT_B];

    // Step 1.  Cool down every cell a little
    for (int i = 0; i < CNT_B; i++)
    {
      heatB[i] = qsub8(heatB[i], random8(0, ((COOLING * 10) / CNT_B) + 2));
    }

    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for (int k = CNT_B - 1; k >= 2; k--)
    {
      heatB[k] = (heatB[k - 1] + heatB[k - 2] + heatB[k - 2]) / 3;
    }

    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if (random8() < SPARKING)
    {
      int y = random8(7);
      heatB[y] = qadd8(heatB[y], random8(160, 255));
    }

    // Step 4.  Map from heat cells to LED colors
    for (int j = 0; j < CNT_B; j++)
    {
      CRGB color = HeatColor(heatB[j]);
      int pixelnumber;
      if (gReverseDirection)
      {
        pixelnumber = (CNT_B - 1) - j;
      }
      else
      {
        pixelnumber = j;
      }
      ledsB[pixelnumber] = color;
    }
  }
    
  FastLED.show();
  return true; });
}
