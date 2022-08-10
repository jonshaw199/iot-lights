
#include <Arduino.h>

#include "state/state.h"
#include "state/stateManager.h"
#include "stateent/espent/master/master.h"
#include "stateent/ota/ota.h"
#include "stateent/espent/slave/slave.h"
#include "stateent/base/base.h"
#include "stateent/restart/restart.h"
#include "stateent/idle/idle.h"
#include "stateent/handshake/master/masterHandshake.h"
#include "stateent/handshake/slave/slaveHandshake.h"
#include "stateent/init/init.h"
#include "stateent/purg/purg.h"

Base *espEnt, *ota, *restart, *idle, *handshake, *initEnt, *stateEnt;
Purg *purg;

void setup()
{
  Serial.begin(JS_BAUD);

  // Initializing StateManager as early as possible
  StateManager::init(STATE_INIT);
  // Constructing init since it's actually used right now (we are technically in STATE_INIT now so we should call setup below)
  initEnt = new Init();
  // Point stateEnt to init since it's the active state entity
  stateEnt = initEnt;
  // General setup can be handled in Init setup/loop methods
  stateEnt->setup();
  // Construct the rest of the global variables so they will be ready for use later (setup should happen on these later in loop() when we switch to these states)
  ota = new OTA();
  restart = new Restart();
  idle = new Idle();
  purg = new Purg();
#if MASTER
  espEnt = new Master();
  handshake = new MasterHandshake();
#else
  espEnt = new Slave();
  handshake = new SlaveHandshake();
#endif

  StateManager::setRequestedState(STATE_HANDSHAKE);
}

void loop()
{
  // Handling this first instead of last; allows us to use init.loop() if we need it before switching to the requested state (or maybe we don't want to request a new state during init at all?)
  stateEnt->loop();

  // Check if state change requested and proceed if stateEnt->preStateChange() says its ok
  JSState curState = StateManager::getCurState();
  JSState requestedState = StateManager::getRequestedState();
  if (curState != requestedState)
  {
    Serial.println("Requested state change: " + StateManager::stateToString(requestedState));
    if (stateEnt->preStateChange(requestedState))
    {
      StateManager::changeToRequestedState();

      switch (requestedState)
      {
      case STATE_INIT:
        stateEnt = initEnt;
        break;
      case STATE_OTA:
        stateEnt = ota;
        break;
      case STATE_RUN:
        stateEnt = espEnt;
        break;
      case STATE_RESTART:
        stateEnt = restart;
        break;
      case STATE_IDLE:
        stateEnt = idle;
        break;
      case STATE_HANDSHAKE:
        stateEnt = handshake;
        break;
      case STATE_PURG_OTA:
        stateEnt = purg;
        purg->setNext(STATE_OTA);
        purg->setPurgMs(10000);
        break;
      }

      stateEnt->setup();
    }
    else
    {
      Serial.println("State change rejected by preStateChange");
    }
  }

  // Handling user input
  if (Serial.available() > 0)
  {
    String s = Serial.readString();
    StateManager::handleUserInput(s);
  }
}
