#include "demo3.h"
#include "state.h"

bool Demo3::on = false;
unsigned long Demo3::showtime = 0;

void Demo3::start()
{
    StateManager::getCurStateEnt()->getIntervalEventMap().insert(std::pair<String, IntervalEvent>("Demo3_Start", IntervalEvent(
                                                                                                                     300, [](IECBArg a)
                                                                                                                     {
            Serial.print(a.getCbCnt());
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
    Serial.print("; diff: ");
    Serial.println(showtime - millis());

    unsigned long dif = showtime - millis();
    unsigned long intervalMs = dif + StateManager::getCurStateEnt()->getElapsedMs();

    StateManager::getCurStateEnt()->getIntervalEventMap().insert(std::pair<String, IntervalEvent>("Demo3_ScheduleStart", IntervalEvent(
                                                                                                                             intervalMs, [](IECBArg a)
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
                                                                                        3000, [](IECBArg a)
                                                                                        {
            showtime = millis() + (unsigned long)6000;
            
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
                Serial.print("Received time: ");
                Serial.println(d.ms);
                showtime = StateManager::convertTime(m.getSenderID(), d.ms);
                Serial.print("Converted time: ");
                Serial.println(showtime);
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

void Demo3::preStateChange(int s)
{
    Base::preStateChange(s);
    setBuiltinLED(0);
}
