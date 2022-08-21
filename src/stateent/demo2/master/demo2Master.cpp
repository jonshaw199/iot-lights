
#include "demo2Master.h"
#include "state.h"
#include "led/led.h"
#include "stateEnt/demo2/demo2.h"

const float Demo2Master::coefs[] = {0, .1, .3, .5, .7, .9, 1, 1, .9, .7, .5, .3, .1, 0, 0};
const unsigned long Demo2Master::sceneMs = 10000;

Demo2Master::Demo2Master()
{
  intervalEvents.push_back(IntervalEvent(MS_DEMO2_LOOP, demo2));
}

bool Demo2Master::demo2(IECBArg a)
{
  Serial.print("ElapsedMs: ");
  Serial.println(a.getElapsedMs());
  getCurCoef(a.getElapsedMs());
  return true;
}

void Demo2Master::setup()
{
  sendMsg(0);
}

void Demo2Master::sendMsg(uint8_t b)
{
  JSMessage msg;
  msg.setState(STATE_DEMO1);
  msg.setType(TYPE_RUN_DATA);
  demo2_data d;
  d.brightness = b;
  msg.setData((uint8_t *)&d);
  MessageHandler::pushOutbox(msg);
}

// Smooths out transitions from one coef to the next
float Demo2Master::getCurCoef(unsigned long elapsedMs)
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
