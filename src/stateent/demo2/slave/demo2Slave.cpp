
#include "demo2Slave.h"
#include "led/led.h"
#include "stateEnt/demo2/demo2.h"

void Demo2Slave::setup()
{
  Base::setup();
  LED::init();
}

bool Demo2Slave::preStateChange(int s)
{
  bool baseResult = Base::preStateChange(s);
  if (baseResult)
  {
    Serial.println("Turning off lights on the way out");
    LED::fillColor(CRGB::Black);
  }
  return baseResult;
}

bool Demo2Slave::handleInboxMsg(JSMessage m)
{
  switch (m.getType())
  {
  case TYPE_RUN_DATA:
    demo2_data d;
    memcpy(&d, m.getData(), sizeof(d));
    LED::setBrightness(d.brightness);
    LED::fillColor(d.color);
    return true;
  }

  return Base::handleInboxMsg(m);
}

void Demo2Slave::setInboxMessageHandler()
{
  MessageHandler::setInboxMsgHandler(handleInboxMsg);
}
