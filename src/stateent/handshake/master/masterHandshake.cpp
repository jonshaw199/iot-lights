#include "masterHandshake.h"
#include "message/messageHandler.h"
#include "util/wifi/wifiUtil.h"
#include "state/stateManager.h"

void MasterHandshake::setup()
{
  WifiUtil::prepareWifi();
  WiFi.mode(WIFI_AP);
  delay(DELAY_PREPARE_WIFI);
  MessageHandler::initEspNow();
}

void MasterHandshake::loop()
{
  Base::loop();

  // Check if handshake has been completed for all slaves
  int numHandshakeComplete = 0;
  for (std::map<int, js_peer_info>::const_iterator it = MessageHandler::getPeerInfoMap().begin(); it != MessageHandler::getPeerInfoMap().end(); it++)
  {
    if (it->second.handshakeResponse)
    {
      numHandshakeComplete++;
    }
  }
  if (numHandshakeComplete >= SLAVE_CNT)
  {
    StateManager::setRequestedState(INITIAL_STATE);
  }

  MessageHandler::scanForPeers();
  MessageHandler::connectToPeers();

  for (std::map<int, js_peer_info>::const_iterator it = MessageHandler::getInstance().getPeerInfoMap().begin(); it != MessageHandler::getInstance().getPeerInfoMap().end() && !it->second.handshakeRequest; it++)
  {
    MessageHandler::sendHandshakeRequests({it->first});
  }

  delay(DELAY_MASTER_HANDSHAKE_LOOP);
}