#ifndef STATEENT_MASTER_MASTER_H_
#define STATEENT_MASTER_MASTER_H_

#include "stateent/base/base.h"

class Master : public Base
{
public:
  void setup();
  void loop();
  bool preStateChange(JSState s);
};

#endif // STATEENT_MASTER_MASTER_H_