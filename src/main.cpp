
#include <Arduino.h>

#include "master/master.h"
#include "ota/ota.h"
#include "slave/slave.h"
#include "state/state.h"
#include "state/stateManager.h"
#include "base/base.h"
#include "restart/restart.h"

Base *espEnt;
OTA ota;
Restart restart;

Base *stateEnt;

void setup()
{
  Serial.begin(115200);

  // initialize the built-in-LED pin as an output:
  pinMode(LED_BUILTIN, OUTPUT);

  // setupDisplay();

  ota = OTA();
  restart = Restart();

  if (MASTER)
  {
    espEnt = new Master();
  }
  else
  {
    espEnt = new Slave();
  }

  espEnt->setup();
  stateEnt = espEnt;

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
      }

      stateEnt->setup();
    }
    else
    {
      Serial.println("State change rejected by preStateChange");
    }
  }

  if (stateEnt != NULL)
  {
    stateEnt->loop();
  }
}
