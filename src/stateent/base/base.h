#ifndef STATEENT_BASE_BASE_H_
#define STATEENT_BASE_BASE_H_

#include <Arduino.h>

#include "state/state.h"

class Base
{
public:
  Base();
  virtual void setup();
  virtual void loop();
  virtual bool preStateChange(JSState s);
};

#endif // STATEENT_BASE_BASE_H_