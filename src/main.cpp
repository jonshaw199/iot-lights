
#include <Arduino.h>

#include <framework.h>

#ifdef MASTER
#include "stateEnt/demo1/master/demo1Master.h"
#include "stateEnt/demo2/master/demo2Master.h"
#else
#include "stateEnt/demo1/slave/demo1Slave.h"
#include "stateEnt/demo2/slave/demo2Slave.h"
#endif
#include "state.h"

void setStateDemo1()
{
  StateManager::getInstance().setRequestedState(STATE_DEMO1);
}

void setStateDemo2()
{
  StateManager::getInstance().setRequestedState(STATE_DEMO2);
}

void setup()
{
  Serial.begin(JS_BAUD);
  Framework::setup();
#ifdef MASTER
  Framework::registerStateEnt(STATE_DEMO1, new Demo1Master(), "STATE_DEMO1");
  Framework::registerStateEnt(STATE_DEMO2, new Demo2Master(), "STATE_DEMO2");
#else
  Framework::registerStateEnt(STATE_DEMO1, new Demo1Slave(), "STATE_DEMO1");
  Framework::registerStateEnt(STATE_DEMO2, new Demo2Slave(), "STATE_DEMO2");
#endif
  Framework::registerStringHandler("1", setStateDemo1);
  Framework::registerStringHandler("2", setStateDemo2);
}

void loop()
{
  Framework::loop();
}
