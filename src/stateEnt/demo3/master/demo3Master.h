#ifndef STATEENT_DEMO3_MASTER_DEMO3MASTER_H_
#define STATEENT_DEMO3_MASTER_DEMO3MASTER_H_

#include <framework.h>

class demo3Master : public Base
{
  static bool demo3(IECBArg a);
  static void sendMsg(uint8_t b);
  static const float coefs[];
  static const unsigned long sceneMs;
  static float getCurCoef(unsigned long elapsedMs);
  static const uint8_t maxBrightness;

public:
  demo3Master();
  void setup();
};

#endif // STATEENT_DEMO3_MASTER_DEMO3MASTER_H_