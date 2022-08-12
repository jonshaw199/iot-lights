#include "masterHandshake.h"
#include "message/messageHandler.h"
#include "util/wifi/wifiUtil.h"
#include "state/stateManager.h"
#include "stateent/base/intervalEvent/intervalEvent.h"

bool handleHandshakes()
{
  MessageHandler::scanForPeers();
  MessageHandler::connectToPeers();
  MessageHandler::sendAllHandshakes();
  return true;
}

void MasterHandshake::setup()
{
  Base::setup();
  WifiUtil::prepareWifi();
  WiFi.mode(WIFI_AP);
  delay(DELAY_PREPARE_WIFI);
  MessageHandler::initEspNow();
  intervalEvents.push_back(IntervalEvent(MS_MASTER_HANDSHAKE_LOOP, handleHandshakes));
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
}