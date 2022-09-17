
#include "demo1.h"
#include "state.h"
#include "led/led.h"

typedef struct demo1_data
{
  int r;
  int g;
  int b;
} demo1_data;

Demo1::Demo1()
{
#if MASTER
  intervalEventMap.insert(std::pair<String, IntervalEvent>("Demo1", IntervalEvent(MS_DEMO1_LOOP, [](IECBArg a)
                                                                                  {
  AF1Msg msg;
  msg.setState(STATE_DEMO1);
  msg.setType(TYPE_RUN_DATA);
  msg.setMaxRetries(MS_DEMO1_LOOP >= 1000 ? 3 : 0);
  msg.getJson()["r"] = rand() % 250;
  msg.getJson()["g"] = rand() % 250;
  msg.getJson()["b"] = rand() % 250;
  pushOutbox(msg);

  return true; })));
#endif
}

void Demo1::setup()
{
  Base::setup();
  JSLED::init();
  JSLED::setBrightness(10);
}

void Demo1::preStateChange(int s)
{
  Base::preStateChange(s);
  Serial.println("Turning off lights on the way out");
  JSLED::fillColor(CRGB::Black);
}

msg_handler Demo1::getInboxHandler()
{
  return [](AF1Msg m)
  {
    Base::handleInboxMsg(m);
    if (m.getState() == STATE_DEMO1)
    {
      switch (m.getType())
      {
      case TYPE_RUN_DATA:
        CRGB c(m.getJson()["r"], m.getJson()["g"], m.getJson()["b"]);
        JSLED::fillColor(c);
        break;
      }
    }
  };
}

String Demo1::getName()
{
  return "STATE_DEMO1";
}

void Demo1::serializeESPNow(AF1Msg &m)
{
  demo1_data d = {m.getJson()["r"], m.getJson()["g"], m.getJson()["b"]};
  m.setData((uint8_t *)&d);
}

void Demo1::deserializeESPNow(AF1Msg &m)
{
  demo1_data d;
  memcpy(&d, m.getData(), sizeof(d));
  m.getJson()["r"] = d.r;
  m.getJson()["g"] = d.g;
  m.getJson()["b"] = d.b;
}
