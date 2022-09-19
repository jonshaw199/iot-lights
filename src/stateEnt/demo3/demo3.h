#ifndef STATEENT_DEMO3_DEMO3_H_
#define STATEENT_DEMO3_DEMO3_H_

#include <Arduino.h>
#include <AF1.h>

typedef struct demo3_data
{
    unsigned long long ms;
} demo3_data;

class Demo3 : public Base
{
    static void start();
    static void scheduleStart();

public:
    Demo3();
    void setup();
    msg_handler getInboxHandler();
    bool doScanForPeersESPNow();
    void preStateChange(int s);
    static bool on;
    static unsigned long long showtime;
};

#endif // STATEENT_DEMO3_DEMO3_H_