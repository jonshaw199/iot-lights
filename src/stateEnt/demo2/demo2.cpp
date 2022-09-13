
#include <FastLED.h>

#include "demo2.h"
#include "state.h"
#include "led/led.h"

typedef struct demo2_data
{
  int r = 0;
  int g = 250;
  int b = 0;
  int brightness = 0;
} demo2_data;

const float Demo2::coefs[] = {0, .00001, .0001, .0003, .0005, .001, .003, .005, .01, .03, .05, .1, .15, .3, .5, .7, .9, 1, 1, 1, .9, .7, .5, .3, .15, .1, .05, .03, .01, .005, .003, .001, .0005, 0};
const unsigned long Demo2::sceneMs = 7000;
const uint8_t Demo2::maxBrightness = 150;

Demo2::Demo2()
{
#if MASTER
  intervalEventMap.insert(std::pair<String, IntervalEvent>("Demo2", IntervalEvent(MS_DEMO2_LOOP, [](IECBArg a)
                                                                                  {
  AF1Msg msg;
  msg.setState(STATE_DEMO2);
  msg.setType(TYPE_RUN_DATA);
  msg.getJson()["brightness"] = getCurCoef(a.getElapsedMs()) * maxBrightness;
  pushOutbox(msg);
  return true; })));
#endif
}

void Demo2::setup()
{
  Base::setup();
  JSLED::init();
}

void Demo2::preStateChange(int s)
{
  Base::preStateChange(s);
  Serial.println("Turning off lights on the way out");
  JSLED::fillColor(CRGB::Black);
}

void Demo2::overrideInboxHandler()
{
  setInboxMsgHandler([](AF1Msg m)
                     {
    Base::handleInboxMsg(m);
    switch (m.getType()) {
    case TYPE_RUN_DATA:   
      JSLED::setBrightness(m.getJson()["brightness"]); 
      CRGB c(m.getJson()["r"], m.getJson()["g"], m.getJson()["b"]);
      JSLED::fillColor(c);
      break;
    } });
}

String Demo2::getName()
{
  return "STATE_DEMO2";
}

void Demo2::serializeESPNow(AF1Msg &m)
{
  demo2_data d;
  d.brightness = m.getJson()["brightness"];
  m.setData((uint8_t *)&d);
}

void Demo2::deserializeESPNow(AF1Msg &m)
{
  demo2_data d;
  memcpy(&d, m.getData(), sizeof(d));
  m.getJson()["r"] = d.r;
  m.getJson()["g"] = d.g;
  m.getJson()["b"] = d.b;
  m.getJson()["brightness"] = d.brightness;
}

float Demo2::getCurCoef(unsigned long elapsedMs)
{
  float curSceneMs = elapsedMs % sceneMs;
  float curSceneRatio = curSceneMs / sceneMs;
  float coefArrIdxExact = curSceneRatio * sizeof(coefs) / sizeof(coefs[0]);
  int coefArrIdxTrunc = coefArrIdxExact;
  float coefArrIdxRem = coefArrIdxExact - coefArrIdxTrunc;

  float coefA = coefs[coefArrIdxTrunc];
  float coefB = coefs[coefArrIdxTrunc + 1];
  float min = std::min(coefA, coefB);
  float max = std::max(coefA, coefB);
  float dif = max - min;

  float rem = coefArrIdxRem * dif;
  float result = min == coefA ? min + rem : max - rem;
  return result;
}
