#include "slaveHandshake.h"
#include "message/messageHandler.h"

void SlaveHandshake::loop()
{
  if (MessageHandler::getInbox().size())
  {
    JSMessage m = MessageHandler::getAndPop();
    if (m.getType() == TYPE_HANDSHAKE_REQUEST)
    {
      MessageHandler::receiveHandshakeRequest(m);
      MessageHandler::sendHandshakeResponses({m.getSenderID()});
    }
  }
}