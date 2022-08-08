#include "slave.h"

void Slave::setup()
{
  led = LED();
}

void Slave::loop()
{
  /*
  // Check msg queue for new messages
  if (StateManager::getMsgQueue().size())
  {
    js_message msg;
    // Just taking the most recent msg for now
    while (StateManager::getMsgQueue().size())
    {
      msg = StateManager::dequeMsg();
    }

    if (StateManager::getCurState() == msg.state)
    {
      // When will this not be STATE_RUN here??
      switch (msg.state)
      {
      case STATE_RUN:
        led.fillColor(msg.color);
        break;
      default:
        Serial.print("State not found: ");
        Serial.println(msg.state);
      }
    }
    else
    {
      // State change requested by master
      StateManager::setRequestedState(msg.state);
    }
  }
  */
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