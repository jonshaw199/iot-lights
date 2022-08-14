
#include <Arduino.h>

#include "purg.h"
#include "state/stateManager.h"
#include "message/message.h"
#include "message/messageHandler.h"

Purg::Purg()
{
  purgMs = MS_PURG_DEFAULT;
  next = STATE_OTA;
}

void Purg::setup()
{
  Base::setup();

#ifdef MASTER
  JSMessage msg;
  msg.setType(TYPE_CHANGE_STATE);
  msg.setState(next);
  msg.setMaxRetries(RETRIES_PURG);
  MessageHandler::getOutbox().push(msg);
#endif
}

void Purg::loop()
{
  Base::loop();

  if (getElapsedMs() > purgMs)
  {
    Serial.println("Purgatory over");
    StateManager::setRequestedState(next);
  }
}

void Purg::setPurgMs(unsigned long ms)
{
  purgMs = ms;
}

void Purg::setNext(JSState s)
{
  next = s;
}