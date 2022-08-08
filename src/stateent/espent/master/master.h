#ifndef STATEENT_ESPENT_MASTER_MASTER_H_
#define STATEENT_ESPENT_MASTER_MASTER_H_

#include "stateent/espent/espEnt.h"

class Master : public EspEnt
{
public:
  void loop();
  // bool preStateChange(JSState s);
};

#endif // STATEENT_ESPENT_MASTER_MASTER_H_