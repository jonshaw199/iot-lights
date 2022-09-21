#include "sync.h"
#include "state.h"

unsigned long Sync::showtime = 0;

void Sync::start()
{
    StateManager::getCurStateEnt()->getIntervalEventMap().insert(std::pair<String, IntervalEvent>("Sync_Start", IntervalEvent(
                                                                                                                    300, [](IECBArg a)
                                                                                                                    {
            setBuiltinLED(a.getCbCnt() % 2);
            return true; },
                                                                                                                    -1, true)));
}

void Sync::scheduleStart()
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

    StateManager::getCurStateEnt()->getIntervalEventMap().insert(std::pair<String, IntervalEvent>("Sync_ScheduleStart", IntervalEvent(
                                                                                                                            intervalMs, [](IECBArg a)
                                                                                                                            {
            Serial.println("Starting");
            start();
            return true; },
                                                                                                                            1, true)));
}

Sync::Sync()
{
#if MASTER
    // Schedule send start time
    intervalEventMap.insert(std::pair<String, IntervalEvent>("Sync_Sendshowtimeg", IntervalEvent(
                                                                                       3000, [](IECBArg a)
                                                                                       {
            showtime = millis() + (unsigned long)6000;
            
            AF1Msg msg;
            msg.setState(STATE_SYNC);
            msg.setType(TYPE_RUN_DATA);
            sync_data d;
            d.ms = showtime;
            msg.setData((uint8_t *)&d);
            pushOutbox(msg);
            scheduleStart();
            return true; },
                                                                                       1)));
#endif
}

msg_handler Sync::getInboxHandler()
{
    return [](AF1Msg m)
    {
        Base::handleInboxMsg(m);
        if (m.getState() == STATE_SYNC)
        {
            switch (m.getType())
            {
            case TYPE_RUN_DATA:
                sync_data d;
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

bool Sync::doScanForPeersESPNow()
{
    return false;
}

void Sync::preStateChange(int s)
{
    Base::preStateChange(s);
    setBuiltinLED(0);
}
