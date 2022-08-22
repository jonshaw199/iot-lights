#ifndef STATEENT_DEMO3_SLAVE_DEMO3SLAVE_H_
#define STATEENT_DEMO3_SLAVE_DEMO3SLAVE_H_

#include <framework.h>

class demo3Slave : public Base
{
public:
  void setup();
  bool preStateChange(int s);
  static bool handleInboxMsg(JSMessage m);
  void setInboxMessageHandler();
};

#endif // STATEENT_DEMO3_SLAVE_DEMO1SLAVE_H_