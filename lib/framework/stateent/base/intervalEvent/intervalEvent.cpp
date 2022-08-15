#include "intervalEvent.h"

IntervalEvent::IntervalEvent()
{
  intervalMs = 0;
  maxCbCnt = 0;
}

IntervalEvent::IntervalEvent(unsigned long i, interval_event_cb c)
{
  intervalMs = i;
  cb = c;
  maxCbCnt = -1;
}

IntervalEvent::IntervalEvent(unsigned long i, interval_event_cb c, int m)
{
  intervalMs = i;
  cb = c;
  maxCbCnt = m;
}

unsigned long IntervalEvent::getIntervalMs()
{
  return intervalMs;
}

interval_event_cb IntervalEvent::getCb()
{
  return cb;
}

int IntervalEvent::getMaxCbCnt()
{
  return maxCbCnt;
}

int IntervalEvent::getCbCnt()
{
  return cbCnt;
}

unsigned long IntervalEvent::getLastCbMs()
{
  return intervalMs * cbCnt;
}

unsigned long IntervalEvent::getNextCbMs()
{
  return getLastCbMs() + intervalMs;
}

bool IntervalEvent::isTime(unsigned long elapsedMs)
{
  return elapsedMs >= getNextCbMs() && (maxCbCnt < 0 || cbCnt < maxCbCnt);
}

bool IntervalEvent::cbIfTime(unsigned long elapsedMs)
{
  if (isTime(elapsedMs) && intervalMs && cb()) // Checking intervalMs here since default constructor doesnt even define cb; might need stub there to be safe
  {
    cbCnt++;
    return true;
  }
  return false;
}
