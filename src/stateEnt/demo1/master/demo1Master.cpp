
#include "demo1Master.h"
#include "state.h"
#include "led/led.h"

Demo1Master::Demo1Master() : Base()
{
  intervalEventMap.insert(std::pair<String, IntervalEvent>("Demo1Master_1", IntervalEvent(MS_DEMO1_LOOP, [](IECBArg a)
                                                                                          {
  AF1Msg msg;
  msg.setState(STATE_DEMO1);
  msg.setType(TYPE_RUN_DATA);
  msg.setMaxRetries(MS_DEMO1_LOOP >= 1000 ? 3 : 0);
  CRGB c = JSLED::getRandColor();
  msg.setData((uint8_t *)&c);
  pushOutbox(msg);

  return true; })));
}
