#include "demo4.h"
#include "led/led.h"

void Demo4::setup()
{
  WSEnt::setup();
  JSLED::init();
}

bool Demo4::preStateChange(int s)
{
  bool baseResult = WSEnt::preStateChange(s);
  if (baseResult)
  {
    Serial.println("Turning off lights on the way out");
    JSLED::fillColor(CRGB::Black);
  }
  return baseResult;
}

bool Demo4::handleInboxMsg(JSMessage m)
{
  switch (m.getType())
  {
  case TYPE_RUN_DATA:
    demo4_data d;
    memcpy(&d, m.getData(), sizeof(d));
    JSLED::setBrightness(d.brightness);
    JSLED::fillColor(d.color);
    return true;
  }

  return WSEnt::handleInboxMsg(m);
}

void Demo4::setInboxMessageHandler()
{
  setInboxMsgHandler(handleInboxMsg);
}
