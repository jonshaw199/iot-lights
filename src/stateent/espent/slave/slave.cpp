#include "slave.h"
#include "messageHandler/messageHandler.h"
#include "led/led.h"
#include "stateManager/stateManager.h"

void Slave::setup()
{
  Base::setup();
  LED::init();
}

bool Slave::preStateChange(JSState s)
{
  bool baseResult = Base::preStateChange(s);
  if (baseResult)
  {
    Serial.println("Leaving STATE_RUN so turning off lights");
    LED::fillColor(CRGB::Black);
  }
  return baseResult;
}

void Slave::handleRunData(JSMessage m)
{
  LED::fillColor(m.getColor());
}

bool Slave::handleInboxMsg(JSMessage m)
{
  switch (m.getType())
  {
  case TYPE_RUN_DATA:
    handleRunData(m);
    return true;
  }

  return Base::handleInboxMsg(m);
}

void Slave::setInboxMessageHandler()
{
  MessageHandler::setInboxMsgHandler(handleInboxMsg);
}
