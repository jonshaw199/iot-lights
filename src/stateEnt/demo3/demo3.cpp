#include <Arduino.h>
#include <AF1.h>

#include "state.h"

typedef struct demo3_data
{
    unsigned long long ms;
} demo3_data;

bool on = false;
unsigned long long showtime = 0;

class Demo3 : public Base
{
    static void start()
    {
        StateManager::getCurStateEnt()->getIntervalEventMap().insert(std::pair<String, IntervalEvent>("Demo3_Start", IntervalEvent(300, [](IECBArg a)
                                                                                                                                   {
                Serial.println("Do");
                on = !on;
                setBuiltinLED(on);
                return true; })));
    }

    static void scheduleStart()
    {
        StateManager::getCurStateEnt()->getIntervalEventMap().insert(std::pair<String, IntervalEvent>("Demo3_ScheduleStart", IntervalEvent(
                                                                                                                                 showtime, [](IECBArg a)
                                                                                                                                 {
            start();
            return true; },
                                                                                                                                 1)));
    }

public:
    Demo3()
    {
#if MASTER
        showtime = millis() + 5000;

        // Schedule send start time
        intervalEventMap.insert(std::pair<String, IntervalEvent>("Demo3_Sendshowtimeg", IntervalEvent(
                                                                                            3000, [](IECBArg a)
                                                                                            {
            AF1Msg msg;
            msg.setState(STATE_DEMO3);
            msg.setType(TYPE_RUN_DATA);
            demo3_data d;
            d.ms = showtime;
            msg.setData((uint8_t *)&d);
            pushOutbox(msg);
            return true; },
                                                                                            1)));

        scheduleStart();
#endif
    }

    msg_handler getInboxHandler()
    {
        return [](AF1Msg m)
        {
            Base::handleInboxMsg(m);
            if (m.getState() == STATE_DEMO3)
            {
                switch (m.getType())
                {
                case TYPE_RUN_DATA:
                    demo3_data d;
                    memcpy(&d, m.getData(), sizeof(d));
                    showtime = d.ms;
                    //
                    break;
                }
            }
        };
    }

    bool doScanForPeersESPNow()
    {
        return false;
    }
};