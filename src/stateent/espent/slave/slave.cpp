#include "slave.h"
#include "message/messageHandler.h"
#include "led/led.h"

void Slave::setup()
{
  // led = LED();
  LED::init();
}

void Slave::loop()
{

  if (MessageHandler::getInbox().size())
  {
    JSMessage m = MessageHandler::popAndFrontInbox();
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
    Serial.println("Leaving STATE_RUN so turning off lights");
    LED::fillColor(CRGB::Black);
  }
  return baseResult;
}

void Slave::handleRunData(JSMessage m)
{
  LED::fillColor(m.getColor());
}
