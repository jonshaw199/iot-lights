
#include <AF1.h>

#ifdef ARDUINO_M5Stick_C
#include <M5StickCPlus.h> // https://github.com/m5stack/M5Stack/issues/97
#undef min
#endif

#include "state.h"
#include "stateEnt/home/home.h"
#include "stateEnt/song1/song1.h"

void setup()
{
  Serial.setTimeout(JS_SERIAL_TIMEOUT);
  Serial.begin(JS_BAUD);
#ifdef ARDUINO_M5Stick_C
  M5.begin();
#endif
  AF1::setup(JS_ID);
#ifdef JS_IP_A
  AF1::registerWifiAP(JSSSID, JSPASS, JS_IP_A, JS_IP_B, JS_IP_C, JS_IP_D, 192, 168, 1, 254, 255, 255, 255, 0);
#else
  AF1::registerWifiAP(JSSSID, JSPASS);
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
