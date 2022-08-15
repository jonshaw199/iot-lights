#include "stateManager/stateManager.h"
#include "messageHandler/messageHandler.h"
#include "framework.h"

void Framework::setup()
{
  StateManager::init();
  MessageHandler::init();
}

void Framework::loop()
{
  MessageHandler::loop();
  StateManager::loop();
}

void Framework::registerStateEnt(int i, Base *s)
{
  StateManager::registerStateEnt(i, s);
}

void Framework::registerStringHandler(String s, string_input_handler h)
{
  StateManager::registerStringHandler(s, h);
}
