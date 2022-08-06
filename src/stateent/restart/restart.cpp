#include "restart.h"

void Restart::loop()
{
  Serial.println("Restart looping (restarting)...");
  ESP.restart();
}