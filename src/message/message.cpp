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
  msg.msgID = msgID++;
  recipients = {};
}

void JSMessage::setState(JSState s)
{
  msg.state = s;
}

void JSMessage::setColor(CRGB c)
{
  msg.color = c;
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