#include "masterHandshake.h"
#include "message/messageHandler.h"

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