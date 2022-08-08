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
  int msgID;
  // Required
  MessageType type;
  int senderID;
  JSState state;
  // State dependent
  uint8_t senderAPMac[6]; // Handshake
  CRGB color;             // Run
} js_message;

// This class is a wrapper around the js_message struct that actually gets sent using ESPNOW
class JSMessage
{
  js_message msg;
  std::set<int> recipients;
  int sendCnt;
  static int msgID;
  int retries;
  int maxRetries;

public:
  JSMessage();
  void setRecipients(std::set<int> r);
  std::set<int> getRecipients();
  js_message asStruct();
  int incrementSendCnt();
  int getSendCnt();
  void setType(MessageType t);
  MessageType getType();
  void setState(JSState s);
  JSState getState();
  void setColor(CRGB c);
  CRGB getColor();
  void setSenderAPMac(uint8_t *m);
  uint8_t *getSenderAPMac();
  int getMsgID();
  void setSenderID(int id);
  int getSenderID();
  void setRetries(int r);
  int getRetries();
  int incrementRetries();
  void setMaxRetries(int m);
  int getMaxRetries();
};

#endif // MESSAGE_MESSAGE_H_
