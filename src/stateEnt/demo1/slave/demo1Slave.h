#ifndef STATEENT_DEMO1_SLAVE_DEMO1SLAVE_H_
#define STATEENT_DEMO1_SLAVE_DEMO1SLAVE_H_

#include <AF1.h>

class Demo1Slave : public Base
{
public:
  void setup();
  void preStateChange(int s);
  static void handleInboxMsg(AF1Msg m);
  void overrideInboxHandler();
};

#endif // STATEENT_DEMO1_SLAVE_DEMO1SLAVE_H_