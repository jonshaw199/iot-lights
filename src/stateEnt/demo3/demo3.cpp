#include "demo3.h"
#include "state.h"

bool Demo3::on = false;
unsigned long long Demo3::showtime = 0;

void Demo3::start()
{
    StateManager::getCurStateEnt()->getIntervalEventMap().insert(std::pair<String, IntervalEvent>("Demo3_Start", IntervalEvent(300, [](IECBArg a)
                                                                                                                               {
            Serial.println("Do");
            on = !on;
            setBuiltinLED(on);
            return true; })));
}

void Demo3::scheduleStart()
{
    StateManager::getCurStateEnt()->getIntervalEventMap().insert(std::pair<String, IntervalEvent>("Demo3_ScheduleStart", IntervalEvent(
                                                                                                                             showtime, [](IECBArg a)
                                                                                                                             {
            start();
            return true; },
                                                                                                                             1)));
}

Demo3::Demo3()
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

msg_handler Demo3::getInboxHandler()
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
                showtime = StateManager::convertTime(m.getSenderID(), d.ms);
                scheduleStart();
                break;
            }
        }
    };
}

bool Demo3::doScanForPeersESPNow()
{
    return false;
}
