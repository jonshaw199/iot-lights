#ifndef STATEENT_TEST_BLINK_BLINK_H_
#define STATEENT_TEST_BLINK_BLINK_H_

#include <AF1.h>

class Blink : public Base
{
public:
  Blink();
  void setup();
  void preStateChange(int s);
  msg_handler getInboxHandler();
  void serializeESPNow(AF1Msg &m);
  void deserializeESPNow(AF1Msg &m);
  String getName();
};

#endif // STATEENT_TEST_BLINK_BLINK_H_
