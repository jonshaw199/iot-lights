#ifndef STATEENT_DEMO2_DEMO2_H_
#define STATEENT_DEMO2_DEMO2_H_

#include <AF1.h>

class Demo2 : public Base
{
  static const float coefs[];
  static const unsigned long sceneMs;
  static const uint8_t maxBrightness;
  static float getCurCoef(unsigned long elapsedMs);

public:
  Demo2();
  void setup();
  void preStateChange(int s);
  void overrideInboxHandler();
  void serializeESPNow(AF1Msg &m);
  void deserializeESPNow(AF1Msg &m);
  String getName();
  bool scanForESPNowPeers();
};

#endif // STATEENT_DEMO2_DEMO2_H_
