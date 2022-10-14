#ifndef STATEENT_RC3_H_
#define STATEENT_RC3_H_

#include <AF1.h>

#include <M5StickCPlus.h>
#undef min // https://github.com/m5stack/M5Stack/issues/97

#include "state.h"

class RC3 : public Base
{
protected:
  static float accX;
  static float accY;
  static float accZ;
  static double theta;
  static double last_theta;
  static double phi;
  static double last_phi;
  static double alpha;

public:
  RC3(ws_client_info i);
  void setup();
  void preStateChange(int s);
};

#endif
