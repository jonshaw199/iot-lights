
#include <Arduino.h>

#include <AF1.h>

#ifdef MASTER
#include "stateEnt/demo1/master/demo1Master.h"
#include "stateEnt/demo2/master/demo2Master.h"
#else
#include "stateEnt/demo1/slave/demo1Slave.h"
#include "stateEnt/demo2/slave/demo2Slave.h"
#endif
#include "state.h"
#include "stateEnt/demo4/demo4.h"

void setup()
{
  Serial.begin(JS_BAUD);
  AF1::setup();
  AF1::registerWifiAP("js-guest", "B1g5lams!", JS_IP_A, JS_IP_B, JS_IP_C, JS_IP_D, 192, 168, 1, 254, 255, 255, 255, 0);
  AF1::registerWSServer("192.168.1.65", "/ws", 3000);
#ifdef MASTER
  AF1::registerStateEnt(STATE_DEMO1, new Demo1Master(), "STATE_DEMO1");
  AF1::registerStateEnt(STATE_DEMO2, new Demo2Master(), "STATE_DEMO2");
#else
  AF1::registerStateEnt(STATE_DEMO1, new Demo1Slave(), "STATE_DEMO1");
  AF1::registerStateEnt(STATE_DEMO2, new Demo2Slave(), "STATE_DEMO2");
#endif
  AF1::registerStateEnt(STATE_DEMO4, new Demo4(), "STATE_DEMO4");
  AF1::registerStringHandler("1", []()
                             { AF1::setRequestedState(STATE_DEMO1); });
  AF1::registerStringHandler("2", []()
                             { AF1::setRequestedState(STATE_DEMO2); });
  AF1::registerStringHandler("4", []()
                             { AF1::setRequestedState(STATE_DEMO4); });
  AF1::setInitialState(STATE_IDLE_WS);
}

void loop()
{
  AF1::loop();
}
