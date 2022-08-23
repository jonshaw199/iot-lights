#ifndef STATEENT_DEMO3_MASTER_DEMO3MASTER_H_
#define STATEENT_DEMO3_MASTER_DEMO3MASTER_H_

#include <framework.h>

class Demo3Master : public ESPNowEntMaster
{
  static bool demo3(IECBArg a);

public:
  Demo3Master();
};

#endif // STATEENT_DEMO3_MASTER_DEMO3MASTER_H_