
#include "demo3Master.h"
#include "state.h"
#include "led/led.h"
#include "stateEnt/demo3/demo3.h"

const float demo3Master::coefs[] = {0, .00001, .0001, .0003, .0005, .001, .003, .005, .01, .03, .05, .1, .15, .3, .5, .7, .9, 1, 1, 1, .9, .7, .5, .3, .15, .1, .05, .03, .01, .005, .003, .001, .0005, 0, 0, 0};
const unsigned long demo3Master::sceneMs = 10000;
const uint8_t demo3Master::maxBrightness = 100;

demo3Master::demo3Master()
{
  intervalEvents.push_back(IntervalEvent(MS_DEMO3_LOOP, demo3));
}

bool demo3Master::demo3(IECBArg a)
{
  uint8_t brightness = getCurCoef(a.getElapsedMs()) * maxBrightness;
  sendMsg(brightness);
  return true;
}

void demo3Master::setup()
{
  sendMsg(0);
}

void demo3Master::sendMsg(uint8_t b)
{
  JSMessage msg;
  msg.setState(STATE_DEMO1);
  msg.setType(TYPE_RUN_DATA);
  demo3_data d;
  d.brightness = b;
  msg.setData((uint8_t *)&d);
  MessageHandler::pushOutbox(msg);
}

// Smooths out transitions from one coef to the next
float demo3Master::getCurCoef(unsigned long elapsedMs)
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
