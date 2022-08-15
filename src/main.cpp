
#include <Arduino.h>

#include <framework.h>

#ifdef MASTER
#include "stateent/demo1/master/demo1Master.h"
#else
#include "stateent/demo1/slave/demo1Slave.h"
#endif
#include "state.h"

Framework framework;

void setStateDemo1()
{
  StateManager::getInstance().setRequestedState(STATE_DEMO1);
}

void setup()
{
  Serial.begin(JS_BAUD);
  framework.setup();
#ifdef MASTER
  framework.registerStateEnt(STATE_DEMO1, new Demo1Master());
#else
  framework.registerStateEnt(STATE_DEMO1, new Demo1Slave());
#endif
  framework.registerStringHandler("r", setStateDemo1);
}

void loop()
{
  framework.loop();
}
