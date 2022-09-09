
#include "demo1Slave.h"
#include "led/led.h"

void Demo1Slave::setup()
{
  Base::setup();
  JSLED::init();
}

bool Demo1Slave::validateStateChange(int s)
{
  bool baseResult = ESPNowEnt::validateStateChange(s);
  if (baseResult)
  {
    Serial.println("Turning off lights on the way out");
    JSLED::fillColor(CRGB::Black);
  }
  return baseResult;
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
