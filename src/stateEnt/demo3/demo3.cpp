#include "demo3.h"
#include "state.h"

bool Demo3::on = false;
unsigned long long Demo3::showtime = 0;

void Demo3::start()
{
    StateManager::getCurStateEnt()->getIntervalEventMap().insert(std::pair<String, IntervalEvent>("Demo3_Start", IntervalEvent(
                                                                                                                     300, [](IECBArg a)
                                                                                                                     {
            on = !on;
            setBuiltinLED(on);
            return true; },
                                                                                                                     -1, true)));
}

void Demo3::scheduleStart()
{
    Serial.println("Scheduling");
    Serial.print("Current time: ");
    Serial.print(millis());
    Serial.print("; Showtime: ");
    Serial.print(showtime);

    unsigned long long rem = showtime - StateManager::getCurStateEnt()->getStartMs();
    Serial.print("; rem: ");
    Serial.println(rem);

    StateManager::getCurStateEnt()->getIntervalEventMap().insert(std::pair<String, IntervalEvent>("Demo3_ScheduleStart", IntervalEvent(
                                                                                                                             rem, [](IECBArg a)
                                                                                                                             {
            Serial.println("Starting");
            start();
            return true; },
                                                                                                                             1, true)));
}

Demo3::Demo3()
{
#if MASTER
    // Schedule send start time
    intervalEventMap.insert(std::pair<String, IntervalEvent>("Demo3_Sendshowtimeg", IntervalEvent(
                                                                                        6000, [](IECBArg a)
                                                                                        {
            AF1Msg msg;
            msg.setState(STATE_DEMO3);
            msg.setType(TYPE_RUN_DATA);
            demo3_data d;
            d.ms = showtime;
            msg.setData((uint8_t *)&d);
            pushOutbox(msg);
            scheduleStart();
            return true; },
                                                                                        1)));
#endif
}

void Demo3::setup()
{
    Base::setup();
#if MASTER
    showtime = millis() + (unsigned long long)6000;
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
