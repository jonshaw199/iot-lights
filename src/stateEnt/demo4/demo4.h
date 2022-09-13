#ifndef STATEENT_DEMO4_DEMO4_H_
#define STATEENT_DEMO4_DEMO4_H_

#include <AF1.h>

class Demo4 : public Base
{
public:
  Demo4(ws_client_info i);
  void setup();
  void preStateChange(int s);
  static void handleInboxMsg(AF1Msg m);
  void overrideInboxHandler();
  String getName();
};

#endif // STATEENT_DEMO4_DEMO4_H_
