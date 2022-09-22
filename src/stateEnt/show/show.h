#ifndef STATEENT_SHOW_SHOW_H_
#define STATEENT_SHOW_SHOW_H_

#include <AF1.h>

class Show : public Base
{
public:
  String getName();
  bool doScanForPeersESPNow();
  bool doConnectToWSServer();
};

#endif