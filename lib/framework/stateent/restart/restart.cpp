#include "restart.h"
#include "state/state.h"

void Restart::loop()
{
  if (preStateChange(STATE_NONE))
  {
    Serial.println("Restarting now...");
    delay(1000);
    ESP.restart();
  }
  else
  {
    Serial.println("Restart blocked by preStateChange");
  }
}