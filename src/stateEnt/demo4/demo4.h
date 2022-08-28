#ifndef STATEENT_DEMO4_DEMO4_H_
#define STATEENT_DEMO4_DEMO4_H_

#include <framework.h>
#include <FastLED.h>

typedef struct demo4_data
{
  CRGB color = CRGB::Red;
  uint8_t brightness = 0;
} demo4_data;

class Demo4 : public WSEnt
{
  void setup();
  bool preStateChange(int s);
  static bool handleInboxMsg(JSMessage m);
  void setInboxMessageHandler();
};

#endif // STATEENT_DEMO4_DEMO4_H_
