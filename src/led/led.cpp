
#include "led.h"

LED::LED()
{
  if (CNT_A)
  {
    FastLED.addLeds<WS2812, LED_PIN_A, GRB>(ledsA, CNT_A);
  }
  if (CNT_B)
  {
    FastLED.addLeds<WS2812, LED_PIN_B, GRB>(ledsB, CNT_B);
  }
  FastLED.setBrightness(200);
}

void LED::fillColor(CRGB color)
{
  Serial.println("Filling color");
  if (CNT_A)
  {
    fill_solid(ledsA, CNT_A, color);
  }
  if (CNT_B)
  {
    fill_solid(ledsB, CNT_B, color);
  }
  FastLED.show();
}

CRGB LED::getRandColor()
{
  CRGB options[] = {CRGB::Red, CRGB::Blue, CRGB::Green, CRGB::White};
  int i = rand() % 4;
  return options[i];
}