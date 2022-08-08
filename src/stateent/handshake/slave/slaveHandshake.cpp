#include "slaveHandshake.h"
#include "message/messageHandler.h"
#include "util/wifi/wifiUtil.h"

void SlaveHandshake::setup()
{
  WifiUtil::setAPMode();
  WifiUtil::broadcastAP();
  MessageHandler::initEspNow();
}

void SlaveHandshake::loop()
{
  if (MessageHandler::getInbox().size())
  {
    JSMessage m = MessageHandler::getAndPop();
    switch (m.getType())
    {
    case TYPE_HANDSHAKE_REQUEST:
      Serial.println("Handshake request message in inbox");
      MessageHandler::receiveHandshakeRequest(m);
      MessageHandler::sendHandshakeResponses({m.getSenderID()});
      break;
    default:
      Serial.println("Not a message we care about");
    }
  }
}
