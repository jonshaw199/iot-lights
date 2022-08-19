
#include "demo2Master.h"
#include "state.h"
#include "led/led.h"
#include "stateEnt/demo2/demo2.h"

Demo2Master::Demo2Master()
{
  intervalEvents.push_back(IntervalEvent(MS_DEMO2_LOOP, demo2));
}

bool Demo2Master::demo2()
{

  return true;
}

void Demo2Master::setup()
{
  sendMsg(0);
}

void Demo2Master::sendMsg(uint8_t b)
{
  JSMessage msg;
  msg.setState(STATE_DEMO1);
  msg.setType(TYPE_RUN_DATA);
  demo2_data d;
  d.brightness = b;
  msg.setData((uint8_t *)&d);
  MessageHandler::pushOutbox(msg);
}
