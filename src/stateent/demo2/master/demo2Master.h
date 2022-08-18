#ifndef STATEENT_DEMO2_MASTER_DEMO2MASTER_H_
#define STATEENT_DEMO2_MASTER_DEMO2MASTER_H_

#include <framework.h>

class Demo2Master : public Base
{
  static bool demo2();
  static void sendMsg(uint8_t b);

public:
  Demo2Master();
  void setup();
};

#endif // STATEENT_DEMO2_MASTER_DEMO2MASTER_H_