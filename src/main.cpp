
#include <AF1.h>

#include "state.h"
#include "stateEnt/home/home.h"
#include "stateEnt/show/show.h"
#include "stateEnt/test/sync/sync.h"

void setup()
{
  Serial.begin(JS_BAUD);
  Serial.setTimeout(JS_SERIAL_TIMEOUT);
  AF1::setup(JS_ID);
#ifdef JS_IP_A
  AF1::registerWifiAP("js-guest", "B1g5lams!", JS_IP_A, JS_IP_B, JS_IP_C, JS_IP_D, 192, 168, 1, 254, 255, 255, 255, 0);
#else
  AF1::registerWifiAP("js-guest", "B1g5lams!");
#endif
  AF1::registerStateEnt(STATE_HOME, new Home());
  AF1::registerStateEnt(STATE_SHOW, new Show());
  AF1::registerStateEnt(STATE_SYNC, new Sync());
  AF1::registerStringHandler("home", []()
                             { AF1::setRequestedState(STATE_HOME); });
  AF1::registerStringHandler("show", []()
                             { AF1::setRequestedState(STATE_SHOW); });
  AF1::registerStringHandler("sync", []()
                             { AF1::setRequestedState(STATE_SYNC); });
}

void loop()
{
  AF1::loop();
}
