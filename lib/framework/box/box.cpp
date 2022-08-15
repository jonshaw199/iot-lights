
#include <Arduino.h>

#include "box/box.h"

bool dummyHandler(JSMessage m)
{
  return false;
}

Box::Box()
{
  msgHandler = dummyHandler;
}

void Box::handleMessages()
{
  m.lock();
  if (!q.empty())
  {
    JSMessage msg = q.front();
    q.pop();
    m.unlock();
    msgHandler(msg);
    handleMessages();
  }
  else
  {
    m.unlock();
  }
}

void Box::setMsgHandler(msg_handler m)
{
  msgHandler = m;
}
