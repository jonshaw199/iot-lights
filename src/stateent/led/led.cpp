
#include "led.h"

void LED::setup()
{
  Serial.println("LED setting up");

  switch (JS_ID)
  {
  case 2:
    ledsA = ledsBack;
    ledsB = ledsPatio;
    ledsCntA = LED_CNT_BACK;
    ledsCntB = LED_CNT_PATIO;
    break;
  case 3:
    ledsA = ledsKitchen;
    ledsCntA = LED_CNT_KITCHEN;
    break;
  case 4:
    ledsA = ledsGarageA;
    ledsB = ledsGarageB;
    ledsCntA = LED_CNT_GARAGE_A;
    ledsCntB = LED_CNT_GARAGE_B;
    break;
  case 5:
    ledsA = ledsFront;
    ledsCntA = LED_CNT_FRONT;
    break;
  default:
    Serial.println("No assigned LEDs");
    break;
  }

  if (ledsA != NULL)
  {
    FastLED.addLeds<WS2812, LED_PIN_A, GRB>(ledsA, ledsCntA);
  }
  if (ledsB != NULL)
  {
    FastLED.addLeds<WS2812, LED_PIN_B, GRB>(ledsB, ledsCntB);
  }
  FastLED.setBrightness(200);
}

void LED::fillColor(CRGB color)
{
  Serial.println("Filling color");
  if (ledsA != NULL)
  {
    fill_solid(ledsA, ledsCntA, color);
  }
  if (ledsB != NULL)
  {
    fill_solid(ledsB, ledsCntB, color);
  }
  FastLED.show();
}

CRGB LED::getRandColor()
{
  CRGB options[] = {CRGB::Red, CRGB::Blue, CRGB::Green, CRGB::White};
  int i = rand() % 4;
  return options[i];
}

bool LED::preStateChange(JSState s)
{
  if (s != STATE_RUN)
  {
    // Turn off lights
    fillColor(CRGB::Black);
  }
  return true;
}