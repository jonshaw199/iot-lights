
#include <Arduino.h>
#include <AF1.h>

#include "state.h"
#include "stateEnt/home/home.h"

void setup()
{
  Serial.begin(JS_BAUD);
  AF1::setup(JS_ID);
#ifdef JS_IP_A
  AF1::registerWifiAP("js-guest", "B1g5lams!", JS_IP_A, JS_IP_B, JS_IP_C, JS_IP_D, 192, 168, 1, 254, 255, 255, 255, 0);
#else
  AF1::registerWifiAP("js-guest", "B1g5lams!");
#endif
  AF1::registerStateEnt(STATE_HOME, new Home());
  AF1::registerStringHandler("1", []()
                             { AF1::setRequestedState(STATE_HOME); });
}

void loop()
{
  AF1::loop();
}
