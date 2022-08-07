#include "init.h"
#include "message/messageHandler.h"
#include "state/stateManager.h"

void Init::setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  // setupDisplay();

  MessageHandler::init();
}