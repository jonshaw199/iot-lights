#include "message.h"

JSMessage::JSMessage()
{
  msg = {};
  recipients = {};
}

JSMessage::JSMessage(JSState s, CRGB c)
{
  msg = {s, c};
  recipients = {};
}

JSMessage::JSMessage(js_message m)
{
  msg = m;
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