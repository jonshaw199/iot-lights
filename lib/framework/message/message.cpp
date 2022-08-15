#include "message.h"

int JSMessage::msgID = -1;

JSMessage::JSMessage()
{
  msg = {};
  msg.state = STATE_NONE;
  msg.msgID = msgID++;
  msg.senderID = JS_ID;
  msg.color = CRGB::Black;
  msg.type = TYPE_NONE;
  recipients = {};
  sendCnt = 0;
  retries = 0;
  maxRetries = 0;
}

JSMessage::JSMessage(js_message m)
{
  msg = m;
  recipients = {};
  sendCnt = 0;
  retries = 0;
  maxRetries = 0;
}

js_message JSMessage::asStruct()
{
  return msg;
}

std::set<int> JSMessage::getRecipients()
{
  return recipients;
}

void JSMessage::setRecipients(std::set<int> r)
{
  recipients = r;
}

int JSMessage::incrementSendCnt()
{
  return sendCnt++;
}

int JSMessage::getSendCnt()
{
  return sendCnt;
}

void JSMessage::setType(MessageType t)
{
  msg.type = t;
}

MessageType JSMessage::getType()
{
  return msg.type;
}

void JSMessage::setState(int s)
{
  msg.state = s;
}

int JSMessage::getState()
{
  return msg.state;
}

void JSMessage::setColor(CRGB c)
{
  msg.color = c;
}

CRGB JSMessage::getColor()
{
  return msg.color;
}

void JSMessage::setSenderAPMac(uint8_t *m)
{
  memcpy(&msg.senderAPMac, m, 6);
}

int JSMessage::getMsgID()
{
  return msg.msgID;
}

void JSMessage::setSenderID(int id)
{
  msg.senderID = id;
}

int JSMessage::getSenderID()
{
  return msg.senderID;
}

uint8_t *JSMessage::getSenderAPMac()
{
  return msg.senderAPMac;
}

void JSMessage::setMaxRetries(int m)
{
  maxRetries = m;
}

int JSMessage::getMaxRetries()
{
  return maxRetries;
}
