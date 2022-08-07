#ifndef STATEENT_HANDSHAKE_SLAVE_SLAVEHANDSHAKE_H_
#define STATEENT_HANDSHAKE_SLAVE_SLAVEHANDSHAKE_H_

#include "stateent/handshake/handshake.h"

class SlaveHandshake : public Handshake
{
public:
  void setup();
  void loop();
};

#endif // STATEENT_HANDSHAKE_SLAVE_SLAVEHANDSHAKE_H_