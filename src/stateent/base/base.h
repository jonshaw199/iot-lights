#ifndef STATEENT_BASE_BASE_H_
#define STATEENT_BASE_BASE_H_

#include <Arduino.h>
#include <vector>

#include "intervalEvent/intervalEvent.h"
#include "state/state.h"
#include "message/message.h"

class Base
{
protected:
  unsigned long startMs;
  std::vector<IntervalEvent> intervalEvents;
  static bool handleInboxMsg(JSMessage m);
  static bool handleOutboxMsg(JSMessage m);

public:
  Base();
  virtual void setup();
  virtual void loop();
  virtual bool preStateChange(JSState s);
  virtual void setInboxMessageHandler();
  virtual void setOutboxMessageHandler();
  unsigned long getElapsedMs();
};

#endif // STATEENT_BASE_BASE_H_