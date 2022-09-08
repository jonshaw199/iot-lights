#ifndef STATEENT_DEMO1_SLAVE_DEMO1SLAVE_H_
#define STATEENT_DEMO1_SLAVE_DEMO1SLAVE_H_

#include <AF1.h>

class Demo1Slave : public ESPNowEnt
{
public:
  void setup();
  bool validateStateChange(int s);
  static bool handleInboxMsg(AF1Msg m);
  void overrideInboxHandler();
};

#endif // STATEENT_DEMO1_SLAVE_DEMO1SLAVE_H_