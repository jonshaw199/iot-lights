#include <Arduino.h>

#include "master.h"
#include "message/message.h"
#include "message/messageHandler.h"
#include "led/led.h"

void Master::loop()
{
  Base::loop();

  JSMessage msg;
  msg.setState(STATE_RUN);
  msg.setType(TYPE_RUN_DATA);
  msg.setColor(LED::getRandColor());
  msg.setRetries(DEFAULT_RETRIES);

  MessageHandler::sendMsg(msg);

  delay(DELAY_MASTER_LOOP);
}