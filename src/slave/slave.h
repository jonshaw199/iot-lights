#ifndef SLAVE_SLAVE_H_
#define SLAVE_SLAVE_H_

#include "led/led.h"
#include "message/message.h"
#include "base/base.h"

class Slave : public Base
{
  LED led;
  void configDeviceAP();
  void initESPNow();
  static void onDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len);

public:
  Slave();
  void setup();
  void loop();
  bool preStateChange(JSState s);
};

#endif // SLAVE_SLAVE_H_