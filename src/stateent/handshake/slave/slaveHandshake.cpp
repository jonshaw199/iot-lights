#include "slaveHandshake.h"
#include "messageHandler/messageHandler.h"
#include "util/wifi/wifiUtil.h"

void SlaveHandshake::setup()
{
  Base::setup();
  WifiUtil::setAPMode();
  WifiUtil::broadcastAP();
  MessageHandler::initEspNow();
}

void SlaveHandshake::loop()
{
  Base::loop();
}
