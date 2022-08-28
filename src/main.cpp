
#include <Arduino.h>

#include <framework.h>

#ifdef MASTER
#include "stateEnt/demo1/master/demo1Master.h"
#include "stateEnt/demo2/master/demo2Master.h"
#include "stateEnt/demo3/master/demo3Master.h"
#else
#include "stateEnt/demo1/slave/demo1Slave.h"
#include "stateEnt/demo2/slave/demo2Slave.h"
#include "stateEnt/demo3/slave/demo3Slave.h"
#endif
#include "state.h"
#include "stateEnt/demo4/demo4.h"

void setStateDemo1()
{
  StateManager::getInstance().setRequestedState(STATE_DEMO1);
}

void setStateDemo2()
{
  StateManager::getInstance().setRequestedState(STATE_DEMO2);
}

void setStateDemo3()
{
  StateManager::getInstance().setRequestedState(STATE_DEMO3);
}

void setStateDemo4()
{
  StateManager::getInstance().setRequestedState(STATE_DEMO4);
}

void setup()
{
  Serial.begin(JS_BAUD);
  Framework::setup();
#ifdef MASTER
  Framework::registerStateEnt(STATE_DEMO1, new Demo1Master(), "STATE_DEMO1");
  Framework::registerStateEnt(STATE_DEMO2, new Demo2Master(), "STATE_DEMO2");
  Framework::registerStateEnt(STATE_DEMO3, new Demo3Master(), "STATE_DEMO3");
#else
  Framework::registerStateEnt(STATE_DEMO1, new Demo1Slave(), "STATE_DEMO1");
  Framework::registerStateEnt(STATE_DEMO2, new Demo2Slave(), "STATE_DEMO2");
  Framework::registerStateEnt(STATE_DEMO3, new Demo3Slave(), "STATE_DEMO3");
#endif
  Framework::registerStateEnt(STATE_DEMO4, new Demo4(), "STATE_DEMO4");
  Framework::registerStringHandler("1", setStateDemo1);
  Framework::registerStringHandler("2", setStateDemo2);
  Framework::registerStringHandler("3", setStateDemo3);
  Framework::registerStringHandler("4", setStateDemo4);
}

void loop()
{
  Framework::loop();
}
