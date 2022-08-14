#include <Arduino.h>

#include "master.h"
#include "message/message.h"
#include "messageHandler/messageHandler.h"
#include "led/led.h"

bool lightCb1()
{
  JSMessage msg;
  msg.setState(STATE_RUN);
  msg.setType(TYPE_RUN_DATA);
  msg.setColor(LED::getRandColor());
  msg.setMaxRetries(MS_MASTER_LOOP >= 1000 ? DEFAULT_RETRIES : 0);

  MessageHandler::pushOutbox(msg);

  return true;
}

Master::Master()
{
  intervalEvents.push_back(IntervalEvent(MS_MASTER_LOOP, lightCb1));
}
