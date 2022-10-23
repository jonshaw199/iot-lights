#ifndef STATEENT_VIRTUAL_LIGHTSHOWBASE_LIGHTSHOWBASE_H_
#define STATEENT_VIRTUAL_LIGHTSHOWBASE_LIGHTSHOWBASE_H_

#include <AF1.h>

class LightShowBase : public Base
{
public:
  void setup();
  void loop();
  bool doScanForPeersESPNow();
  void onConnectWSServer();
#ifdef VS1053_CS_PIN
  void pausePlaying(bool p);
  void stopPlaying();
#endif
};

#endif