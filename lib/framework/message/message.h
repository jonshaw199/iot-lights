#ifndef MESSAGE_MESSAGE_H_
#define MESSAGE_MESSAGE_H_

#include <Arduino.h>
#include <FastLED.h>
#include <set>

#include "state/state.h"

enum MessageType
{
  TYPE_NONE,
  TYPE_HANDSHAKE_REQUEST,
  TYPE_HANDSHAKE_RESPONSE,
  TYPE_CHANGE_STATE,
  TYPE_RUN_DATA
};

typedef struct js_message
{
  // Calculated
  // int msgID;
  // Required
  int type;
  int senderID;
  int state;
  // State dependent
  uint8_t senderAPMac[6]; // Handshake

  uint8_t data[100];
} js_message;

// This class is a wrapper around the js_message struct that actually gets sent using ESPNOW
class JSMessage
{
  js_message msg;
  std::set<int> recipients;
  int sendCnt;
  int retries;
  int maxRetries;

public:
  JSMessage();
  JSMessage(js_message m);
  void setRecipients(std::set<int> r);
  std::set<int> getRecipients();
  js_message asStruct();
  int incrementSendCnt();
  int getSendCnt();
  void setType(int t);
  int getType();
  void setState(int s);
  int getState();
  void setSenderAPMac(uint8_t *m);
  uint8_t *getSenderAPMac();
  void setSenderID(int id);
  int getSenderID();
  void setMaxRetries(int m);
  int getMaxRetries();
  void setData(uint8_t *d);
  const uint8_t *getData();
};

#endif // MESSAGE_MESSAGE_H_
