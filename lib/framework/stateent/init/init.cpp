#include "init.h"
#include "messageHandler/messageHandler.h"
#include "stateManager/stateManager.h"

void Init::setup()
{
  Serial.println("Setting up");
  Base::setup();
#ifdef LED_BUILTIN
  pinMode(LED_BUILTIN, OUTPUT);
#endif
  // setupDisplay();
}