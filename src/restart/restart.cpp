#include "restart.h"

void Restart::setup()
{
  // Prepare to restart
  Serial.println("Restart setting up");
}

void Restart::loop()
{
  Serial.println("Restart looping (restarting)...");
  ESP.restart();
}

bool Restart::preStateChange(JSState s)
{
  // Never will be used
  return true;
}