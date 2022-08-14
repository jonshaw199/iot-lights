#ifndef STATEENT_ESPENT_SLAVE_SLAVE_H_
#define STATEENT_ESPENT_SLAVE_SLAVE_H_

#include "stateent/espent/espent.h"
#include "message/message.h"

class Slave : public EspEnt
{
  void configDeviceAP();
  void initESPNow();
  static void onDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len);
  static void handleRunData(JSMessage m);

public:
  void setup();
  bool preStateChange(JSState s);
  static bool handleInboxMsg(JSMessage m);
  void setInboxMessageHandler();
};

#endif // STATEENT_ESPENT_SLAVE_SLAVE_H_