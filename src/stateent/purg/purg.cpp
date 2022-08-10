
#include <Arduino.h>

#include "purg.h"
#include "state/stateManager.h"
#include "message/message.h"
#include "message/messageHandler.h"

Purg::Purg()
{
  purgMs = PURG_MS_DEFAULT;
  Serial.print("purgMs(c)");
  Serial.println(purgMs);
  next = STATE_OTA;
}

void Purg::setup()
{
  startMs = millis();

#ifdef MASTER
  JSMessage msg;
  msg.setType(TYPE_CHANGE_STATE);
  msg.setState(next);
  msg.setMaxRetries(RETRIES_PURG);
  MessageHandler::getOutbox().push(msg);
#endif
}

void Purg::loop()
{
  Base::loop();

  unsigned long nowMs = millis();
  Serial.print("nowMs");
  Serial.println(nowMs);
  Serial.print("startMs");
  Serial.println(startMs);
  unsigned long msElapsed = nowMs - startMs;
  Serial.print("msElapsed");
  Serial.println(msElapsed);

  Serial.print("Seconds elapsed: ");
  Serial.println(msElapsed / 1000);

  Serial.print("purgMs");
  Serial.println(purgMs);

  if (msElapsed > purgMs)
  {
    Serial.print("Purgatory over; moving on to next state: ");
    Serial.println(StateManager::stateToString(next));
    StateManager::setRequestedState(next);
  }
}

void Purg::setPurgMs(unsigned long ms)
{
  purgMs = ms;
}

void Purg::setNext(JSState s)
{
  next = s;
}
