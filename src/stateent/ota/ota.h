#ifndef OTA_OTA_H_
#define OTA_OTA_H_

#include "stateent/base/base.h"

class OTA : public Base
{
public:
  void setup();
  void loop();
  bool preStateChange(JSState s);
};

#endif // OTA_OTA_H_