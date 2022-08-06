
#include <Arduino.h>

#include "state/state.h"
#include "state/stateManager.h"
#include "stateent/master/master.h"
#include "stateent/ota/ota.h"
#include "stateent/slave/slave.h"
#include "stateent/base/base.h"
#include "stateent/restart/restart.h"
#include "stateent/idle/idle.h"
#include "stateent/handshake/masterHandshake.h"
#include "stateent/handshake/slaveHandshake.h"

Base *espEnt;
OTA ota;
Restart restart;
Idle idle;
Handshake *handshake;

Base *stateEnt;

void setup()
{
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT); // initialize the built-in-LED pin as an output

  // setupDisplay();

  ota = OTA();
  restart = Restart();
  idle = Idle();

#if MASTER
  espEnt = new Master();
  handshake = new MasterHandshake();
#else
  espEnt = new Slave();
  handshake = new SlaveHandshake();
#endif

  stateEnt = &idle; // Avoid null pointer and just start at idle?

  StateManager::setRequestedState(STATE_RUN);
}

void loop()
{
  if (Serial.available() > 0)
  {
    String s = Serial.readString();
    StateManager::handleUserInput(s);
  }

  JSState curState = StateManager::getCurState();
  JSState requestedState = StateManager::getRequestedState();
  if (curState != requestedState)
  {
    Serial.println("Requested state change");
    if (stateEnt->preStateChange(requestedState))
    {
      StateManager::changeToRequestedState();

      switch (requestedState)
      {
      case STATE_OTA:
        stateEnt = &ota;
        break;
      case STATE_RUN:
        stateEnt = espEnt;
        break;
      case STATE_RESTART:
        stateEnt = &restart;
        break;
      case STATE_IDLE:
        stateEnt = &idle;
        break;
      case STATE_HANDSHAKE:
        stateEnt = handshake;
        break;
      }

      stateEnt->setup();
    }
    else
    {
      Serial.println("State change rejected by preStateChange");
    }
  }

  stateEnt->loop();
}
