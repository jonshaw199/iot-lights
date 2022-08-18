
#include "demo2Master.h"
#include "state.h"
#include "led/led.h"

Demo2Master::Demo2Master()
{
  intervalEvents.push_back(IntervalEvent(MS_DEMO1_LOOP, demo2));
}

bool Demo2Master::demo2()
{
  JSMessage msg;
  msg.setState(STATE_DEMO1);
  msg.setType(TYPE_RUN_DATA);
  msg.setMaxRetries(MS_DEMO1_LOOP >= 1000 ? 3 : 0);
  CRGB c = LED::getRandColor();
  msg.setData((uint8_t *)&c);
  MessageHandler::pushOutbox(msg);

  return true;
}

void Demo2Master::setup()
{
  Base::setup();
  Serial.print("startMs");
  Serial.println(startMs);
  Serial.print("Elapsed");
  Serial.println(getElapsedMs());
}
