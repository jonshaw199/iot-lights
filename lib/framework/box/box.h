#ifndef BOX_BOX_H_
#define BOX_BOX_H_

#include <tsQueue.h>

#include "message/message.h"

typedef bool (*msg_handler)(JSMessage m);

class Box : public TSQueue<JSMessage>
{
  msg_handler msgHandler;

public:
  Box();
  void handleMessages();
  void setMsgHandler(msg_handler h);
};

#endif // BOX_BOX_H_
