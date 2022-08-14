#include "init.h"
#include "messageHandler/messageHandler.h"
#include "stateManager/stateManager.h"

void Init::setup()
{
  Base::setup();
  Serial.println("Setting up");
  pinMode(LED_BUILTIN, OUTPUT);
  // setupDisplay();
  MessageHandler::init();
}