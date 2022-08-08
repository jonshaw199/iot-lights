
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