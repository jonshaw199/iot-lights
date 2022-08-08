#include "masterHandshake.h"
#include "message/messageHandler.h"
#include "util/wifi/wifiUtil.h"

void MasterHandshake::setup()
{
  WifiUtil::prepareWifi();
  WiFi.mode(WIFI_AP);
  delay(DELAY_PREPARE_WIFI);
  MessageHandler::initEspNow();
}

void MasterHandshake::loop()
{
  if (MessageHandler::getInbox().size())
  {
    Serial.println("Messages in inbox");
    JSMessage m = MessageHandler::getAndPop();
    if (m.getType() == TYPE_HANDSHAKE_RESPONSE)
    {
      Serial.println("Handshake response message in inbox");
      MessageHandler::receiveHandshakeResponse(m);
    }
    else
    {
      Serial.println("Not a message we care about");
    }
  }

  MessageHandler::scanForPeers();
  MessageHandler::connectToPeers();

  for (std::map<int, js_peer_info>::const_iterator it = MessageHandler::getInstance().getPeerInfoMap().begin(); it != MessageHandler::getInstance().getPeerInfoMap().end() && !it->second.handshakeResponse; it++)
  {
    MessageHandler::sendHandshakeRequests({it->first});
  }
}