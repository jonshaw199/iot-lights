#ifndef STATEENT_HANDSHAKE_SLAVEHANDSHAKE_H_
#define STATEENT_HANDSHAKE_SLAVEHANDSHAKE_H_

#include "handshake.h"

class SlaveHandshake : public Handshake
{
public:
  void loop();
};

#endif // STATEENT_HANDSHAKE_SLAVEHANDSHAKE_H_