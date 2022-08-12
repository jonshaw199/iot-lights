#ifndef STATEENT_BASE_BASE_H_
#define STATEENT_BASE_BASE_H_

#include <Arduino.h>
#include <vector>

#include "intervalEvent/intervalEvent.h"
#include "state/state.h"

class Base
{
protected:
  unsigned long startMs;
  std::vector<IntervalEvent> intervalEvents;

public:
  Base();
  virtual void setup();
  virtual void loop();
  virtual bool preStateChange(JSState s);
  unsigned long getElapsedMs();
};

#endif // STATEENT_BASE_BASE_H_