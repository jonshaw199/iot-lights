
#include "base.h"
#include "state/stateManager.h"
#include "message/messageHandler.h"

Base::Base()
{
  // Do nothing by default; some children will need to setup(), etc.
}

void Base::setup()
{
}

void Base::loop()
{
  if (MessageHandler::getInbox().size())
  {
    JSMessage m = MessageHandler::getAndPop();

    switch (m.getType())
    {
    case TYPE_CHANGE_STATE:
      Serial.println("State change request message in inbox");
      MessageHandler::receiveHandshakeRequest(m);
      MessageHandler::sendHandshakeResponses({m.getSenderID()});
      return;
    case TYPE_HANDSHAKE_REQUEST:
      Serial.println("Handshake request message in inbox");
      MessageHandler::receiveHandshakeRequest(m);
      MessageHandler::sendHandshakeResponses({m.getSenderID()});
      return;
    }

    if (m.getState() != StateManager::getCurState() && m.getState() != StateManager::getRequestedState())
    {
      Serial.println("Implicit state change to " + StateManager::stateToString(m.getState()));
      StateManager::setRequestedState(m.getState());
    }
  }
}

bool Base::preStateChange(JSState s)
{
  if (StateManager::getCurState() != STATE_OTA && s == STATE_OTA || s == STATE_RESTART)
  {
    JSMessage msg;
    msg.setType(TYPE_CHANGE_STATE);
    msg.setState(s);
    msg.setRetries(NUM_RETRIES_SEND);
    MessageHandler::sendMsg(msg);
  }
  return true;
}