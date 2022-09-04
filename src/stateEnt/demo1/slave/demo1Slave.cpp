
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

bool Demo1Slave::handleInboxMsg(JSMessage m)
{
  switch (m.getType())
  {
  case TYPE_RUN_DATA:
    CRGB c;
    memcpy(&c, m.getData(), sizeof(c));
    JSLED::fillColor(c);
    return true;
  }

  return Base::handleInboxMsg(m);
}

void Demo1Slave::overrideInboxHandler()
{
  setInboxMsgHandler(handleInboxMsg);
}
