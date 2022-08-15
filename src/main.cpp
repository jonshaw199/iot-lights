
#include <Arduino.h>

#include <framework.h>

#ifdef MASTER
#include "stateent/demo1/master/demo1Master.h"
#else
#include "stateent/demo1/slave/demo1Slave.h"
#endif
#include "state.h"

void setStateDemo1()
{
  StateManager::getInstance().setRequestedState(STATE_DEMO1);
}

void setup()
{
  Serial.begin(JS_BAUD);
  Framework::setup();
#ifdef MASTER
  Framework::registerStateEnt(STATE_DEMO1, new Demo1Master());
#else
  Framework::registerStateEnt(STATE_DEMO1, new Demo1Slave());
#endif
  Framework::registerStringHandler("r", setStateDemo1);
}

void loop()
{
  Framework::loop();
}
