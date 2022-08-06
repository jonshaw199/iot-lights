#ifndef BASE_BASE_H_
#define BASE_BASE_H_

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

#endif // BASE_BASE_H_