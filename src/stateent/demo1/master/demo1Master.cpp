
#include <led.h>

#include "demo1Master.h"
#include "state.h"

Demo1Master::Demo1Master()
{
  intervalEvents.push_back(IntervalEvent(MS_MASTER_LOOP, demo1));
}

bool Demo1Master::demo1()
{
  JSMessage msg;
  msg.setState(STATE_DEMO1);
  msg.setType(TYPE_RUN_DATA);
  msg.setMaxRetries(MS_MASTER_LOOP >= 1000 ? DEFAULT_RETRIES : 0);
  CRGB c = LED::getRandColor();
  msg.setData((uint8_t *)&c);
  MessageHandler::pushOutbox(msg);

  return true;
}