#ifndef STATEENT_ESPENT_SLAVE_SLAVE_H_
#define STATEENT_ESPENT_SLAVE_SLAVE_H_

#include "stateent/espent/espent.h"
#include "message/message.h"
#include "led/led.h"

class Slave : public EspEnt
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

#endif // STATEENT_ESPENT_SLAVE_SLAVE_H_