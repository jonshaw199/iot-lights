#ifndef STATEENT_DEMO3_DEMO3_H_
#define STATEENT_DEMO3_DEMO3_H_

#include <Arduino.h>
#include <AF1.h>

typedef struct demo3_data
{
    unsigned long ms;
} demo3_data;

class Demo3 : public Base
{
    static void start();
    static void scheduleStart();

public:
    Demo3();
    msg_handler getInboxHandler();
    bool doScanForPeersESPNow();
    void preStateChange(int s);
    static bool on;
    static unsigned long showtime;
};

#endif // STATEENT_DEMO3_DEMO3_H_