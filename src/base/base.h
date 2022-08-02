#ifndef BASE_BASE_H_
#define BASE_BASE_H_

#include <Arduino.h>

#include "state/state.h"

class Base
{
public:
  Base();
  virtual void setup() = 0;
  virtual void loop() = 0;
  virtual bool preStateChange(JSState s) = 0;
};

#endif // BASE_BASE_H_