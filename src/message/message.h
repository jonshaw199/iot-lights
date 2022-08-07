#ifndef MESSAGE_MESSAGE_H_
#define MESSAGE_MESSAGE_H_

#include <Arduino.h>
#include <FastLED.h>
#include <set>

#include "state/state.h"

enum MessageType
{
  HANDSHAKE,
  STATE_CHANGE,
  UPDATE
};

typedef struct js_message
{
  MessageType type;
  int msgID;
  JSState state;
  CRGB color;
} js_message;

class JSMessage
{
  js_message msg;
  std::set<int> recipients;
  int sendCnt;
  static int msgID;

public:
  JSMessage(js_message m);
  void setState(JSState s);
  void setColor(CRGB c);
  void setRecipients(std::set<int> r);
  const std::set<int> getRecipients();
  js_message asStruct();
  int incrementSendCnt();
  int getSendCnt();
};

#endif // MESSAGE_MESSAGE_H_
