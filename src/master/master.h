#ifndef MASTER_MASTER_H_
#define MASTER_MASTER_H_

#include <esp_now.h>

#include "message/message.h"
#include "base/base.h"

class Master : public Base
{
  esp_now_peer_info_t slaves[SLAVE_CNT];
  int curSlaveCnt = 0;
  static void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
  void initESPNow();
  void scanForSlaves();
  void connectToSlaves();
  void sendData(js_message data);

public:
  void setup();
  void loop();
  bool preStateChange(JSState s);
};

#endif // MASTER_MASTER_H_