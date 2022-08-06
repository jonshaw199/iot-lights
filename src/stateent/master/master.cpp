#include <Arduino.h>

#include "master.h"

void Master::setup()
{
  Serial.println("Master setting up");
}

void Master::loop()
{
  Serial.println("Master looping");

  /*
  if (curSlaveCnt < SLAVE_CNT)
  {
    scanForSlaves();
    connectToSlaves();
  }

  if (curSlaveCnt)
  {
    js_message msg;
    msg.state = StateManager::getCurState();
    msg.color = LED::getRandColor();
    sendData(msg);
  }
  */

  delay(DELAY_MASTER_LOOP);
}

bool Master::preStateChange(JSState s)
{
  /*
  if (StateManager::getCurState() == STATE_RUN && s == STATE_OTA)
  {
    // Inform the slaves so they also switch state
    Serial.println("Informing the slaves of OTA");
    js_message msg;
    msg.state = STATE_OTA;
    sendData(msg);
    delay(3000);
  }
  */
  return true;
}