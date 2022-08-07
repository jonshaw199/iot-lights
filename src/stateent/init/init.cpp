#include "init.h"
#include "message/messageHandler.h"
#include "state/stateManager.h"

void Init::setup()
{
  Serial.println("Setting up");
  pinMode(LED_BUILTIN, OUTPUT);
  // setupDisplay();

  MessageHandler::init();
}