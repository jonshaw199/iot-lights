#include "slave.h"
#include "message/messageHandler.h"

void Slave::setup()
{
  led = LED();
}

void Slave::loop()
{

  if (MessageHandler::getInbox().size())
  {
    JSMessage m = MessageHandler::popAndFront();
    switch (m.getType())
    {
    case TYPE_RUN_DATA:
      handleRunData(m);
      break;
    }
  }

  // Do this LAST because it will clear the inbox currently!!
  Base::loop();
}

bool Slave::preStateChange(JSState s)
{
  bool baseResult = Base::preStateChange(s);
  if (baseResult)
  {
    led.fillColor(CRGB::Black);
  }
  return baseResult;
}

void Slave::handleRunData(JSMessage m)
{
  led.fillColor(m.getColor());
}
