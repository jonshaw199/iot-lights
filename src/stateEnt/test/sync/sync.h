#ifndef STATEENT_TEST_SYNC_H_
#define STATEENT_TEST_SYNC_H_

#include <Arduino.h>
#include <AF1.h>

typedef struct sync_data
{
    unsigned long ms;
} sync_data;

class Sync : public Base
{
    static void start();
    static void scheduleStart();

public:
    Sync();
    msg_handler getInboxHandler();
    bool doScanForPeersESPNow();
    void preStateChange(int s);
    static bool on;
    static unsigned long showtime;
};

#endif // STATEENT_TEST_SYNC_H_