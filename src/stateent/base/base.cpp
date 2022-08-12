
#include "base.h"
#include "state/stateManager.h"
#include "message/messageHandler.h"

Base::Base()
{
}

void Base::setup()
{
  startMs = millis();
}

void Base::loop()
{
  // Outbox
  for (int i = 0; i < MessageHandler::getOutbox().size(); i++)
  {
    JSMessage m = MessageHandler::getOutbox().front();
    MessageHandler::getOutbox().pop();
    MessageHandler::sendMsg(m);
  }

  // Inbox
  for (int i = 0; i < MessageHandler::getInbox().size(); i++)
  {
    JSMessage m = MessageHandler::getInbox().front();
    MessageHandler::getInbox().pop();

    switch (m.getType())
    {
    case TYPE_CHANGE_STATE:
      Serial.println("State change request message in inbox");
      StateManager::setRequestedState(m.getState());
      return;
    case TYPE_HANDSHAKE_REQUEST:
      Serial.println("Handshake request message in inbox");
      MessageHandler::receiveHandshakeRequest(m);
      MessageHandler::sendHandshakeResponses({m.getSenderID()});
      return;
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
  }

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
