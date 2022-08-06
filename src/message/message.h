#ifndef MESSAGE_MESSAGE_H_
#define MESSAGE_MESSAGE_H_

#include <Arduino.h>
#include <FastLED.h>
#include <set>

#include "state/state.h"

typedef struct js_message
{
  JSState state;
  CRGB color;
} js_message;

class JSMessage
{
  js_message msg;
  std::set<int> recipients;

public:
  JSMessage();
  JSMessage(JSState s, CRGB c);
  JSMessage(js_message m);
  void setState(JSState s);
  void setColor(CRGB c);
  void setRecipients(std::set<int> r);
  const std::set<int> getRecipients();
  js_message asStruct();
};

#endif // MESSAGE_MESSAGE_H_
