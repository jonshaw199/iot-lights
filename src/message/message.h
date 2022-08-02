#ifndef MESSAGE_MESSAGE_H_
#define MESSAGE_MESSAGE_H_

#include <Arduino.h>
#include <FastLED.h>

#include "state/state.h"

typedef struct js_message
{
  char a[32];
  int b;
  float c;
  bool d;
  String s;
  JSState state;
  CRGB color;
} js_message;

#endif // MESSAGE_MESSAGE_H_
