
#include "base.h"
#include "stateManager/stateManager.h"
#include "messageHandler/messageHandler.h"

Base::Base()
{
}

void Base::setup()
{
  startMs = millis();
}

void Base::loop()
{
  // Interval events
  for (std::vector<IntervalEvent>::iterator it = intervalEvents.begin(); it != intervalEvents.end(); it++)
  {
    it->cbIfTime(getElapsedMs());
  }
}

bool Base::preStateChange(JSState s)
{
  return true;
}

unsigned long Base::getElapsedMs()
{
  unsigned long nowMs = millis();
  return nowMs - startMs;
}

bool Base::handleInboxMsg(JSMessage m)
{
  switch (m.getType())
  {
  case TYPE_CHANGE_STATE:
    Serial.println("State change request message in inbox");
    StateManager::setRequestedState(m.getState());
    break;
  case TYPE_HANDSHAKE_REQUEST:
    Serial.println("Handshake request message in inbox");
    MessageHandler::receiveHandshakeRequest(m);
    MessageHandler::sendHandshakeResponses({m.getSenderID()});
    break;
  case TYPE_HANDSHAKE_RESPONSE:
    Serial.println("Handshake response message in inbox");
    MessageHandler::receiveHandshakeResponse(m);
    break;
  }

#ifndef MASTER
  if (m.getState() != StateManager::getCurState() && m.getState() != StateManager::getRequestedState())
  {
    Serial.println("Implicit state change to " + StateManager::stateToString(m.getState()));
    StateManager::setRequestedState(m.getState());
  }
#endif

  return true;
}

bool Base::handleOutboxMsg(JSMessage m)
{
  MessageHandler::sendMsg(m);
  return true;
}

void Base::setInboxMessageHandler()
{
  MessageHandler::setInboxMsgHandler(handleInboxMsg);
}

void Base::setOutboxMessageHandler()
{
  MessageHandler::setOutboxMsgHandler(handleOutboxMsg);
}
