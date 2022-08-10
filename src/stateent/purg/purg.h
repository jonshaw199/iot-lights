#ifndef STATEENT_PURG_PURG_H_
#define STATEENT_PURG_PURG_H_

#include "stateent/base/base.h"
#include "state/state.h"

class Purg : public Base
{
  unsigned long startMs;
  unsigned long purgMs;
  JSState next;

public:
  Purg();
  void setup();
  void loop();
  void setPurgMs(unsigned long purgMs);
  void setNext(JSState s);
};

#endif // STATEENT_PURG_PURG_H_