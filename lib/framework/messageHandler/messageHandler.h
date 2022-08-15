#ifndef MESSAGEHANDLER_MESSAGEHANDLER_H_
#define MESSAGEHANDLER_MESSAGEHANDLER_H_

#include <map>
#include <queue>
#include <set>
#include <esp_now.h>
#include <WiFi.h>
#include <Arduino.h>
#include <mutex>

#include "message/message.h"
#include "box/box.h"

typedef struct js_peer_info
{
  esp_now_peer_info_t espnowPeerInfo;
  bool handshakeRequest;
  bool handshakeResponse;
  JSMessage lastMsg;
  std::mutex mutex;
} js_peer_info;

class MessageHandler
{
  MessageHandler(); // constructor
  std::map<int, js_peer_info> peerInfoMap;
  std::map<String, int> macToIDMap;
  uint8_t macAP[6];
  uint8_t macSTA[6];
  Box inbox;
  Box outbox;
  static void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
  static void onDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len);

public:
  static MessageHandler &getInstance();
  static void scanForPeers(bool overwriteExisting = false);
  static void connectToPeers();
  static void initEspNow();
  static void sendHandshakeRequests(std::set<int> ids);
  static void receiveHandshakeRequest(JSMessage m);
  static void sendHandshakeResponses(std::set<int> ids);
  static void receiveHandshakeResponse(JSMessage m);
  static const std::map<int, js_peer_info> &getPeerInfoMap(); // Read only
  static void sendAllHandshakes();
  static std::set<int> getPeerIDs();
  static const TSQueue<JSMessage> &getOutbox(); // Read only
  static const TSQueue<JSMessage> &getInbox();  // Read only
  static void handleInboxMessages();
  static void handleOutboxMessages();
  static void setInboxMsgHandler(msg_handler h);
  static void setOutboxMsgHandler(msg_handler h);
  static void pushOutbox(JSMessage m);
  static void sendMsg(JSMessage msg);
  static void init();
  static void loop();
};

#endif // MESSAGEHANDLER_MESSAGEHANDLER_H_
