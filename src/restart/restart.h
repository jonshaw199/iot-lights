#ifndef RESTART_RESTART_H_
#define RESTART_RESTART_H_

#include "base/base.h"

class Restart : public Base
{
public:
  void setup();
  void loop();
  bool preStateChange(JSState s);
};

#endif // RESTART_RESTART_H