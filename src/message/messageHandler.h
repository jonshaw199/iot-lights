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
  bool handshakeResponse;
  JSMessage lastMsg;
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
  static bool validateMsg(JSMessage m);

public:
  static MessageHandler &getInstance();
  static void scanForPeers();
  static void connectToPeers();
  static void sendMsg(JSMessage msg);
  static void init();
  static void deinit();
  static void initEspNow();
  static void deinitEspNow();
  static std::queue<JSMessage> &getInbox();
  static void sendHandshakeRequests(std::set<int> ids);
  static void receiveHandshakeRequest(JSMessage m);
  static void sendHandshakeResponses(std::set<int> ids);
  static void receiveHandshakeResponse(JSMessage m);
  static const std::map<int, js_peer_info> &getPeerInfoMap(); // Read only
};

#endif // MESSAGE_MESSAGEHANDLER_H_