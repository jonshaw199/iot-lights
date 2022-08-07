#include "message.h"

int JSMessage::msgID = -1;

JSMessage::JSMessage()
{
  msg = {};
  msg.msgID = msgID++;
  recipients = {};
}

JSMessage::JSMessage(js_message m)
{
  msg = m;
  recipients = {};
}

JSMessage::JSMessage(js_message m, std::set<int> r)
{
  msg = m;
  recipients = r;
}

js_message JSMessage::asStruct()
{
  return msg;
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

MessageType JSMessage::getType() { return msg.type; }

void JSMessage::setState(JSState s)
{
  msg.state = s;
}

JSState JSMessage::getState() { return msg.state; }

void JSMessage::setColor(CRGB c)
{
  msg.color = c;
}

CRGB JSMessage::getColor() { return msg.color; }

void JSMessage::setSenderAPMac(uint8_t *m)
{
  memcpy(&msg.senderAPMac, m, 6);
}

int JSMessage::getID() { return msg.msgID; }
