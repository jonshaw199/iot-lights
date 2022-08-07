#ifndef MESSAGE_MESSAGE_H_
#define MESSAGE_MESSAGE_H_

#include <Arduino.h>
#include <FastLED.h>
#include <set>

#include "state/state.h"

enum MessageType
{
  HANDSHAKE_REQUEST,
  HANDSHAKE_RESPONSE,
  STATE_CHANGE,
  DATA
};

typedef struct js_message
{
  int msgID;
  MessageType type;
  JSState state;
  CRGB color;
  uint8_t senderAPMac[6];
} js_message;

class JSMessage
{
  js_message msg;
  std::set<int> recipients;
  int sendCnt;
  static int msgID;

public:
  JSMessage();
  JSMessage(js_message m);
  JSMessage(js_message m, std::set<int> r);
  void setRecipients(std::set<int> r);
  const std::set<int> getRecipients();
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
  int getID();
};

#endif // MESSAGE_MESSAGE_H_
