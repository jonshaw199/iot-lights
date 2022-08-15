#ifndef STATEENT_BASE_INTERVALEVENT_H_
#define STATEENT_BASE_INTERVALEVENT_H_

typedef bool (*interval_event_cb)();

class IntervalEvent
{
  unsigned long intervalMs;
  interval_event_cb cb;
  int maxCbCnt = -1;

  int cbCnt = 0;

public:
  IntervalEvent();
  IntervalEvent(unsigned long i, interval_event_cb c);
  IntervalEvent(unsigned long i, interval_event_cb c, int m);
  unsigned long getIntervalMs();
  int getMaxCbCnt();
  interval_event_cb getCb();

  int getCbCnt();

  unsigned long getLastCbMs();
  unsigned long getNextCbMs();
  bool isTime(unsigned long elapsedMs);
  bool cbIfTime(unsigned long elapsedMs);
};

#endif // STATEENT_BASE_INTERVALEVENT_H_
