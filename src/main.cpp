
#include <Arduino.h>
#include <AF1.h>

#include "state.h"
#include "stateEnt/demo1/demo1.h"
#include "stateEnt/demo2/demo2.h"
#include "stateEnt/demo4/demo4.h"

void setup()
{
  Serial.begin(JS_BAUD);
  AF1::registerWifiAP("js-guest", "B1g5lams!", JS_IP_A, JS_IP_B, JS_IP_C, JS_IP_D, 192, 168, 1, 254, 255, 255, 255, 0);
  AF1::setup(JS_ID);
  AF1::registerStateEnt(STATE_DEMO1, new Demo1());
  AF1::registerStateEnt(STATE_DEMO2, new Demo2());
  Demo4 *d = new Demo4();
  d->setWSClientInfo({"192.168.1.65", "/rc/demo5/ws", 3000, ""});
  AF1::registerStateEnt(STATE_DEMO4, d);
  AF1::registerStateEnt(STATE_DEMO5, d);
  AF1::registerStringHandler("1", []()
                             { AF1::setRequestedState(STATE_DEMO1); });
  AF1::registerStringHandler("2", []()
                             { AF1::setRequestedState(STATE_DEMO2); });
  AF1::registerStringHandler("4", []()
                             { AF1::setRequestedState(STATE_DEMO4); });
  AF1::registerStringHandler("5", []()
                             { AF1::setRequestedMode(AF1::getCurMode() == MODE_BASIC ? MODE_MESH : MODE_BASIC); });
  AF1::setInitialState(STATE_IDLE_BASE);
  AF1::setDefaultWSClientInfo({STRINGIFY(WS_HOST), STRINGIFY(WS_PATH), WS_PORT, STRINGIFY(WS_PROTOCOL)});
}

void loop()
{
  AF1::loop();
}
