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
  Base::loop();
}
