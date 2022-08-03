#ifndef IDLE_IDLE_H_
#define IDLE_IDLE_H_

#include "base/base.h"

class Idle : public Base
{
public:
  void setup();
  void loop();
  bool preStateChange(JSState s);
};

#endif // IDLE_IDLE_H_