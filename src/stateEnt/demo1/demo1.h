#ifndef STATEENT_DEMO1_DEMO1_H_
#define STATEENT_DEMO1_DEMO1_H_

#include <AF1.h>

class Demo1 : public Base
{
public:
  Demo1();
  void setup();
  void preStateChange(int s);
  void overrideInboxHandler();
  void serializeESPNow(AF1Msg &m);
  void deserializeESPNow(AF1Msg &m);
  String getName();
};

#endif // STATEENT_DEMO1_DEMO1_H_
