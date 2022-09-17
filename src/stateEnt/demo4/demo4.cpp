#include "demo4.h"
#include "led/led.h"
#include "state.h"

Demo4::Demo4() {}

Demo4::Demo4(ws_client_info i)
{
  setWSClientInfo(i);
}

void Demo4::setup()
{
  Base::setup();
  JSLED::init();
}

void Demo4::preStateChange(int s)
{
  Base::preStateChange(s);
  Serial.println("Turning off lights on the way out");
  JSLED::fillColor(CRGB::Black);
}

void Demo4::handleInboxMsg(AF1Msg m)
{
  Base::handleInboxMsg(m);
  if (m.getState() == STATE_DEMO4)
  {
    switch (m.getType())
    {
    case TYPE_RUN_DATA:
      uint8_t b = m.getJson()["brightness"];
      JSLED::setBrightness(b);
      uint8_t r = m.getJson()["red"];
      uint8_t g = m.getJson()["green"];
      uint8_t b2 = m.getJson()["blue"];
      JSLED::fillColor(CRGB(r, g, b2));
    }
  }
}

msg_handler Demo4::getInboxHandler()
{
  return handleInboxMsg;
}

String Demo4::getName()
{
  return "STATE_DEMO4";
}
