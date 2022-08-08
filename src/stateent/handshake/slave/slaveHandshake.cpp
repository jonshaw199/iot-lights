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
    Serial.println("Messages in inbox");
    JSMessage m = MessageHandler::getAndPop();
    if (m.getType() == TYPE_HANDSHAKE_REQUEST)
    {
      Serial.println("Handshake message in inbox");
      MessageHandler::receiveHandshakeRequest(m);
      MessageHandler::sendHandshakeResponses({m.getSenderID()});
    }
    else
    {
      Serial.println("Not a message we care about");
    }
  }
}