#include "song2.h"
#include "stateManager/stateManager.h"

CRGB ledsA[CNT_A];
CRGB ledsB[CNT_B];

// Now shared (same with above)
CRGBPalette16 currentPalette;
TBlendType currentBlending;
CRGBPalette16 targetPalette;

// Static
uint8_t hue = 50;
uint8_t saturation = 255;
uint8_t value = 255;

// Stripes
const int DIR_COEF_INIT = 2;
const int DIR_COEF_CRAZY = 11;
int dirCoef = DIR_COEF_INIT;

void Song2::setup()
{
  LightShowBase::setup();
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
  // setupFire();
  // setupNoise();
  // setupBreathing();
  setupLightning();
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

// For updating HSV at runtime (solid color)

void Song2::set()
{
  AF1::getCurStateEnt()->deactivateIntervalEvents();
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

// Reusable (but only used for Stripes right now)

void Song2::setupHalloweenPalette()
{
  CHSV orange = CHSV(18, 255, 225);
  CHSV white = CHSV(30, 150, 225);
  CHSV purple = CHSV(213, 255, 150);
  CHSV green = CHSV(95, 200, 150);

  currentPalette = CHSVPalette16(
      orange, orange, orange, orange,
      purple, purple, purple, purple,
      white, white, white, white,
      green, green, green, green);
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

// Stripes

void Song2::setupStripes()
{
  // currentPalette = RainbowColors_p;
  // currentBlending = LINEARBLEND;
  currentBlending = NOBLEND;
  setupHalloweenPalette();

  AF1::setIE(IntervalEvent(
      "Song2",
      25, [](IECBArg a)
      {
    static uint8_t startIndex = 0;
    startIndex = startIndex + dirCoef; /* motion speed... and direction */
    
    fillFromPalette( startIndex);
    
  FastLED.show(); }));

  /*
  AF1::setIE(IntervalEvent(
      "Song2_Direction",
      15000, [](IECBArg a)
      {
        dirCoef = dirCoef * -1;
  return true; }));
  */

  AF1::setIE(IntervalEvent(
      "Song2_Crazy",
      60000, [](IECBArg a)
      {
    dirCoef = 0;
    AF1::setIE(IntervalEvent(
        "Song2_Crazy_Start",
        AF1::getCurStateEnt()->getElapsedMs() + 3000, [](IECBArg a)
        {
          dirCoef = DIR_COEF_CRAZY;

          AF1::setIE(IntervalEvent(
              "Song2_Crazy_Start_Dir",
              2500, [](IECBArg a)
              { dirCoef = dirCoef * -1; },
              -1, true)); },
        1, true));
    AF1::setIE(IntervalEvent(
        "Song2_Crazy_Pause",
        AF1::getCurStateEnt()->getElapsedMs() + 9000, [](IECBArg a)
        {
            dirCoef = 0;
            AF1::getCurStateEnt()->getIntervalEventMap()["Song2_Crazy_Start_Dir"].deactivate(); },
        1, true));
        AF1::setIE(IntervalEvent(
          "Song2_Crazy_End",
          AF1::getCurStateEnt()->getElapsedMs() + 11000, [](IECBArg a)
          {
      dirCoef = DIR_COEF_INIT;
          }, 1, true)); }));
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
  AF1::setIE(IntervalEvent(
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
    
  FastLED.show(); }));
}

// Noise

void Song2::setupNoise()
{
  /*
  fillnoise8();

  EVERY_N_MILLIS(10) {
    nblendPaletteTowardPalette(currentPalette, targetPalette, 48);          // Blend towards the target palette over 48 iterations.
  }

  EVERY_N_SECONDS(5) {                                                      // Change the target palette to a random one every 5 seconds.
    uint8_t baseC=random8();
    targetPalette = CRGBPalette16(CHSV(baseC+random8(32), 255, random8(128,255)),   // Create palettes with similar colours.
                                  CHSV(baseC+random8(64), 255, random8(128,255)),
                                  CHSV(baseC+random8(96), 192, random8(128,255)),
                                  CHSV(baseC+random8(16), 255, random8(128,255)));
  }

  LEDS.show();
  */

  setupHalloweenPalette();
  setTargetPalette();

  AF1::setIE(IntervalEvent(
      "Song2",
      1, [](IECBArg a)
      { if (CNT_A) fillNoise8(ledsA, CNT_A);
        if (CNT_B) fillNoise8(ledsB, CNT_B);
        FastLED.show(); }));

  AF1::setIE(IntervalEvent(
      "Song2_Blend",
      10, [](IECBArg a)
      {
        nblendPaletteTowardPalette(currentPalette, targetPalette, 48);          // Blend towards the target palette over 48 iterations.
        FastLED.show(); }));

  AF1::setIE(IntervalEvent(
      "Song2_MovingTarget",
      5000, [](IECBArg a)
      { setTargetPalette(a.getCbCnt()); }));
}

void Song2::fillNoise8(CRGB *leds, int cnt)
{

#define scale 30 // Don't change this programmatically or everything shakes.

  for (int i = 0; i < cnt; i++)
  {                                                                      // Just ONE loop to fill up the LED array as all of the pixels change.
    uint8_t index = inoise8(i * scale, millis() / 10 + i * scale);       // Get a value from the noise function. I'm using both x and y axis.
    leds[i] = ColorFromPalette(currentPalette, index, 255, LINEARBLEND); // With that value, look up the 8 bit colour palette value and assign it to the current LED.
  }

} // fillnoise8()

void Song2::setTargetPalette(unsigned int seed)
{
  CHSV purple = CHSV(213, 255, 50);
  CHSV orange = CHSV(5, 255, 200);
  CHSV green = CHSV(96, 255, 200);
  CHSV white = CHSV(5, 10, 200);

  CHSV o[] = {orange, purple};
  srand(seed);
  Serial.print("Random nums (seed: ");
  Serial.print(seed);
  Serial.print("): ");
  uint8_t randOne = rand() % 2;
  uint8_t randTwo = rand() % 2;
  uint8_t randThree = rand() % 2;
  uint8_t randFour = rand() % 2;
  Serial.print(randOne);
  Serial.print(randTwo);
  Serial.print(randThree);
  Serial.println(randFour);
  targetPalette = CRGBPalette16(o[randOne], o[randTwo], o[randThree], o[randFour]);

  // CHSV arr[] = {purple, orange, green, white};
  // CHSV arr[] = {purple, purple, orange, orange};
  // std::random_shuffle(&arr[0], &arr[3]);
  // targetPalette = CRGBPalette16(arr[0], arr[1], arr[2], arr[3], arr[4]);

  /*
  uint8_t baseC = random8();
  targetPalette = CRGBPalette16(CHSV(baseC + random8(32), 255, random8(128, 255)), // Create palettes with similar colours.
                                CHSV(baseC + random8(64), 255, random8(128, 255)),
                                CHSV(baseC + random8(96), 192, random8(128, 255)),
                                CHSV(baseC + random8(16), 255, random8(128, 255)));
                                */
}

// Breathing

void Song2::setupBreathing()
{
  AF1::setIE(IntervalEvent(
      "Song2",
      10, [](IECBArg a)
      { if (CNT_A) breath(ledsA, CNT_A);
        if (CNT_B) breath(ledsB, CNT_B);
        FastLED.show(); }));
}

void Song2::breath(CRGB *arr, int cnt)
{
  static float pulseSpeed = 0.5; // Larger value gives faster pulse.

  uint8_t hueA = 15;      // 15;      // Start hue at valueMin.
  uint8_t satA = 230;     // Start saturation at valueMin.
  float valueMin = 120.0; // Pulse minimum value (Should be less then valueMax).

  uint8_t hueB = 0;       // 95;      // End hue at valueMax.
  uint8_t satB = 255;     // End saturation at valueMax.
  float valueMax = 255.0; // Pulse maximum value (Should be larger then valueMin).

  uint8_t hue = hueA;                                      // Do Not Edit
  uint8_t sat = satA;                                      // Do Not Edit
  float val = valueMin;                                    // Do Not Edit
  uint8_t hueDelta = hueA - hueB;                          // Do Not Edit
  static float delta = (valueMax - valueMin) / 2.35040238; // Do Not Edit

  float dV = ((exp(sin(pulseSpeed * millis() / 2000.0 * PI)) - 0.36787944) * delta);
  val = valueMin + dV;
  hue = map(val, valueMin, valueMax, hueA, hueB); // Map hue based on current val
  sat = map(val, valueMin, valueMax, satA, satB); // Map sat based on current val

  for (int i = 0; i < cnt; i++)
  {
    arr[i] = CHSV(hue, sat, val);

    // You can experiment with commenting out these dim8_video lines
    // to get a different sort of look.
    arr[i].r = dim8_video(arr[i].r);
    arr[i].g = dim8_video(arr[i].g);
    arr[i].b = dim8_video(arr[i].b);
  }

  FastLED.show();
}

// Lightning

// RIP therealhttps://github.com/fibonacci162

// Added a bit of red for halloween, increased gap between flashes, added finale

#define MAX_FLASHES 8
#define MIN_FLASHES 3
#define MIN_FLASH_MS 4
#define MAX_FLASH_MS 10
#define FLASH_DELAY_MS_INITIAL 150
#define FLASH_DELAY_MS_BASE 50
#define FLASH_DELAY_MS_MAX 100
#define FLASHES_FREQ 22
#define FLASHES_FREQ_FINALE 2
#define FINALE_SEC_MIN 8
#define FINALE_SEC_MAX 11
#define FINALE_INTERVAL_MS 45000

int step;
int flashCounter;
int dimmer;
int flashesFreq;

enum lightning_steps
{
  LS_PRE_FLASH,
  LS_POST_FLASH
};

void Song2::setupLightning()
{
  step = 0;
  flashCounter = 0;
  dimmer = 1;
  flashesFreq = FLASHES_FREQ;

  // The first "flash" in a bolt of lightning is the "leader." The leader
  // is usually duller and has a longer delay until the next flash. Subsequent
  // flashes, the "strokes," are brighter and happen at shorter intervals.

  AF1::setIE(IntervalEvent(
      "Song2",
      1, [](IECBArg a)
      { 
        random16_set_seed(a.getElapsedMs() / 1000);
        
        switch (step) {
        case LS_PRE_FLASH:
          if (flashCounter < random8(MIN_FLASHES, MAX_FLASHES)) {
            if(flashCounter == 0) dimmer = 5;     // the brightness of the leader is scaled down by a factor of 5
            else dimmer = random8(1,3);           // return strokes are brighter than the leader
            if (flashCounter == 2) {
              FastLED.showColor(CHSV(0, 255, 255));
              AF1::setIEIntervalMs("Song2", random8(MIN_FLASH_MS + 3, MAX_FLASH_MS + 3));
            }
            else {
              FastLED.showColor(CHSV(255, 0, 255/dimmer));
              // delay(random8(4,10));                 // each flash only lasts 4-10 milliseconds
              AF1::setIEIntervalMs("Song2", random8(MIN_FLASH_MS, MAX_FLASH_MS));
            }
            step = LS_POST_FLASH;
          } else {
            flashCounter = 0;
            // delay(random8(FREQUENCY)*100);          // delay between strikes
            AF1::setIEIntervalMs("Song2", random8(flashesFreq) * 100);
          }
          break;
        case LS_POST_FLASH:
          FastLED.showColor(CHSV(255, 0, 0));
          // if (flashCounter == 0) delay (150);   // longer delay until next flash after the leader
          // delay(50+random8(100));               // shorter delay between strikes
          AF1::setIEIntervalMs("Song2", flashCounter == 0 ? FLASH_DELAY_MS_INITIAL : FLASH_DELAY_MS_BASE + random8(FLASH_DELAY_MS_MAX));
          flashCounter++;
          step = LS_PRE_FLASH;
          break;
        } }));

  AF1::setIE(IntervalEvent(
      "Song2_Finale",
      FINALE_INTERVAL_MS, [](IECBArg a)
      { 
        if (flashesFreq == FLASHES_FREQ) {
          flashesFreq = FLASHES_FREQ_FINALE;
          AF1::setIEIntervalMs("Song2_Finale", random8(FINALE_SEC_MIN, FINALE_SEC_MAX) * 1000);
          setBuiltinLED(1);
          Serial.println("Start");
        } else {
          flashesFreq = FLASHES_FREQ;
          AF1::setIEIntervalMs("Song2_Finale", FINALE_INTERVAL_MS);
          setBuiltinLED(0);
          Serial.println("End");
        } }));
}
