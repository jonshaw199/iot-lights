#include "masterHandshake.h"
#include "message/messageHandler.h"
#include "util/wifi/wifiUtil.h"

void MasterHandshake::setup()
{
  WifiUtil::prepareWifi();
  WiFi.mode(WIFI_STA);
  delay(1000);
  MessageHandler::initEspNow();
}

void MasterHandshake::loop()
{
  MessageHandler::scanForPeers();
  MessageHandler::connectToPeers();

  for (std::map<int, js_peer_info>::const_iterator it = MessageHandler::getInstance().getPeerInfoMap().begin(); it != MessageHandler::getInstance().getPeerInfoMap().end() && !it->second.handshakeResponse; it++)
  {
    MessageHandler::sendHandshakeRequests({it->first});
  }

  delay(DELAY_MASTER_HANDSHAKE_LOOP);
}