#include "song.h"
#include "state.h"

void Song::setup()
{
  LightShowBase::setup();
#define AUTO_SHUTOFF_MIN (24 * 60 /* EOD */ + 7 * 60 /* TZ Offset */)
  if (timeClient.isTimeSet())
  {
    unsigned long curSec = timeClient.getEpochTime();
    unsigned long beginDaySec = curSec - timeClient.getHours() * 60 * 60 - timeClient.getMinutes() * 60 - timeClient.getSeconds();
    unsigned long autoShutoffSec = beginDaySec + AUTO_SHUTOFF_MIN * 60;
    Serial.print("Auto shutoff seconds: ");
    Serial.println(autoShutoffSec);
    set(Event(
        "LightShowBase_AutoShutoff",
        [](ECBArg a)
        {
          setRequestedState(STATE_HOME);
        },
        true, 0, 1, autoShutoffSec, START_EPOCH_SEC));
  }
}