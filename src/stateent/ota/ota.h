#ifndef STATEENT_OTA_OTA_H_
#define STATEENT_OTA_OTA_H_

#include "stateent/base/base.h"

class OTA : public Base
{
public:
  void setup();
  void loop();
  bool preStateChange(JSState s);
};

#endif // STATEENT_OTA_OTA_H_