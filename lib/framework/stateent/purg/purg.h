#ifndef STATEENT_PURG_PURG_H_
#define STATEENT_PURG_PURG_H_

#include "stateent/base/base.h"
#include "state/state.h"

class Purg : public Base
{
  unsigned long purgMs;
  int next;

public:
  Purg();
  Purg(int s);
  void setup();
  void loop();
  void setPurgMs(unsigned long purgMs);
  void setNext(int s);
};

#endif // STATEENT_PURG_PURG_H_