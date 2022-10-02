#ifndef STATEENT_VIRTUAL_SONG_SONG_H_
#define STATEENT_VIRTUAL_SONG_SONG_H_

#include <AF1.h>

class Song : public Sync
{
  bool doScanForPeersESPNow();
  bool doConnectToWSServer();
};

#endif