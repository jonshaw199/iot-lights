#ifndef STATEENT_SLAVE_SLAVE_H_
#define STATEENT_SLAVE_SLAVE_H_

#include "message/message.h"
#include "stateent/led/led.h"
#include "stateent/base/base.h"

class Slave : public Base
{
  LED led;
  void configDeviceAP();
  void initESPNow();
  static void onDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len);

public:
  void setup();
  void loop();
  bool preStateChange(JSState s);
};

#endif // STATEENT_SLAVE_SLAVE_H_