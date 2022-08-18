#ifndef STATEENT_DEMO2_SLAVE_DEMO2SLAVE_H_
#define STATEENT_DEMO2_SLAVE_DEMO2SLAVE_H_

#include <framework.h>

class Demo2Slave : public Base
{
public:
  void setup();
  bool preStateChange(int s);
  static bool handleInboxMsg(JSMessage m);
  void setInboxMessageHandler();
};

#endif // STATEENT_DEMO2_SLAVE_DEMO1SLAVE_H_