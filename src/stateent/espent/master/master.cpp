#include <Arduino.h>

#include "master.h"
#include "message/message.h"
#include "message/messageHandler.h"
// #include "state/stateManager.h"
#include "stateent/led/led.h"

void Master::loop()
{
  JSMessage msg;
  msg.setState(STATE_RUN);
  msg.setType(TYPE_RUN_DATA);
  msg.setColor(LED::getRandColor());

  MessageHandler::sendMsg(msg);

  delay(DELAY_MASTER_LOOP);
}

/*
bool Master::preStateChange(JSState s)
{
  if (StateManager::getCurState() == STATE_RUN && s == STATE_OTA)
  {
    // Inform the slaves so they also switch state
    Serial.println("Informing the slaves of OTA");
    js_message msg;
    msg.state = STATE_OTA;
    sendData(msg);
    delay(3000);
  }
  return true;
}
*/