#ifndef MASTER_MASTER_H_
#define MASTER_MASTER_H_

#include "stateent/base/base.h"

class Master : public Base
{
public:
  void setup();
  void loop();
  bool preStateChange(JSState s);
};

#endif // MASTER_MASTER_H_