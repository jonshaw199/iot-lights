
#include "demo1Slave.h"
#include "led/led.h"

void Demo1Slave::setup()
{
  Base::setup();
  JSLED::init();
}

void Demo1Slave::preStateChange(int s)
{
  Base::preStateChange(s);
  Serial.println("Turning off lights on the way out");
  JSLED::fillColor(CRGB::Black);
}

void Demo1Slave::handleInboxMsg(AF1Msg m)
{
  Base::handleInboxMsg(m);
  switch (m.getType())
  {
  case TYPE_RUN_DATA:
    CRGB c;
    memcpy(&c, m.getData(), sizeof(c));
    JSLED::fillColor(c);
  }
}

void Demo1Slave::overrideInboxHandler()
{
  setInboxMsgHandler(handleInboxMsg);
}

String Demo1Slave::getName()
{
  return "STATE_DEMO1_SLAVE";
}
