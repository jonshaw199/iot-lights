
#include "demo2Slave.h"

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
    CRGB c;
    memcpy(&c, m.getData(), sizeof(c));
    LED::fillColor(c);
    return true;
  }

  return Base::handleInboxMsg(m);
}

void Demo2Slave::setInboxMessageHandler()
{
  MessageHandler::setInboxMsgHandler(handleInboxMsg);
}
