
#include <AF1.h>

#include "state.h"
#include "stateEnt/home/home.h"
#include "stateEnt/song1/song1.h"

void setup()
{
  Serial.begin(JS_BAUD);
  Serial.setTimeout(JS_SERIAL_TIMEOUT);
  AF1::setup(JS_ID);
#ifdef JS_IP_A
  AF1::registerWifiAP(SSID, PASS, JS_IP_A, JS_IP_B, JS_IP_C, JS_IP_D, 192, 168, 1, 254, 255, 255, 255, 0);
#else
  AF1::registerWifiAP(SSID, PASS);
#endif
  AF1::registerStateEnt(STATE_HOME, new Home());
  AF1::registerStateEnt(STATE_SONG1, new Song1());
  AF1::registerStringHandler("home", []()
                             { AF1::setRequestedState(STATE_HOME); });
  AF1::registerStringHandler("song1", []()
                             { AF1::setRequestedState(STATE_SONG1); });
}

void loop()
{
  AF1::loop();
}
