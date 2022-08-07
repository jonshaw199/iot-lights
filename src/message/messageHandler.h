#ifndef MESSAGE_MESSAGEHANDLER_H_
#define MESSAGE_MESSAGEHANDLER_H_

#include <map>
#include <queue>
#include <esp_now.h>
#include <WiFi.h>

#include "message.h"

typedef struct js_peer_info
{
  esp_now_peer_info_t espnowPeerInfo;
  bool connectionAck;
} js_peer_info;

class MessageHandler
{
  MessageHandler(); // constructor
  std::map<int, js_peer_info> peerInfoMap;
  uint8_t *macAP;
  uint8_t *macSTA;
  std::queue<JSMessage> inbox;
  static void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
  static void onDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len);
  static void scanForPeers();
  static void connectToPeers();

public:
  static MessageHandler &getInstance();
  static void sendMsg(const JSMessage msg);
  static void init();
  static void deinit();
  static const std::queue<JSMessage> &getInbox(); // Read only reference
};

#endif // MESSAGE_MESSAGEHANDLER_H_