
#include "blink.h"
#include "state.h"
#include "led/led.h"

typedef struct blink_data
{
  int r;
  int g;
  int b;
} blink_data;

Blink::Blink()
{
#if MASTER
  intervalEventMap.insert(std::pair<String, IntervalEvent>("Blink_1", IntervalEvent(MS_BLINK_LOOP, [](IECBArg a)
                                                                                    {
  AF1Msg msg;
  msg.setState(STATE_BLINK);
  msg.setType(TYPE_RUN_DATA);
  msg.setMaxRetries(MS_BLINK_LOOP >= 1000 ? 3 : 0);
  msg.getJson()["r"] = rand() % 250;
  msg.getJson()["g"] = rand() % 250;
  msg.getJson()["b"] = rand() % 250;
  pushOutbox(msg);

  return true; })));
#endif
}

void Blink::setup()
{
  Base::setup();
  JSLED::init();
  JSLED::setBrightness(10);
}

void Blink::preStateChange(int s)
{
  Base::preStateChange(s);
  Serial.println("Turning off lights on the way out");
  JSLED::fillColor(CRGB::Black);
}

msg_handler Blink::getInboxHandler()
{
  return [](AF1Msg m)
  {
    Base::handleInboxMsg(m);
    if (m.getState() == STATE_BLINK)
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

String Blink::getName()
{
  return "STATE_BLINK";
}

void Blink::serializeESPNow(AF1Msg &m)
{
  blink_data d = {m.getJson()["r"], m.getJson()["g"], m.getJson()["b"]};
  m.setData((uint8_t *)&d);
}

void Blink::deserializeESPNow(AF1Msg &m)
{
  blink_data d;
  memcpy(&d, m.getData(), sizeof(d));
  m.getJson()["r"] = d.r;
  m.getJson()["g"] = d.g;
  m.getJson()["b"] = d.b;
}
