#ifndef STATEENT_ESPENT_MASTER_MASTER_H_
#define STATEENT_ESPENT_MASTER_MASTER_H_

#include "stateent/espent/espEnt.h"
#include "stateent/base/intervalEvent/intervalEvent.h"

class Master : public EspEnt
{
public:
  Master();
  void loop();
};

#endif // STATEENT_ESPENT_MASTER_MASTER_H_