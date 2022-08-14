#include "init.h"
#include "messageHandler/messageHandler.h"
#include "stateManager/stateManager.h"

void Init::setup()
{
  Base::setup();
  Serial.println("Setting up");
#ifdef LED_BUILTIN
  pinMode(LED_BUILTIN, OUTPUT);
#endif
  // setupDisplay();
  MessageHandler::init();
}