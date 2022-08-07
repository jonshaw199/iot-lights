
#include "stateManager.h"

StateManager &StateManager::getInstance()
{
  static StateManager instance; // Guaranteed to be destroyed.
                                // Instantiated on first use.
  return instance;
}

JSState StateManager::getCurState()
{
  return getInstance().curState;
}

JSState StateManager::getPrevState()
{
  return getInstance().prevState;
}

StateManager::StateManager()
{
}

void StateManager::init(JSState s)
{
  getInstance().curState = s;
  getInstance().requestedState = s;
  getInstance().webServer = new AsyncWebServer(80);
}

void StateManager::setRequestedState(JSState s)
{
  getInstance().requestedState = s;
}

JSState StateManager::getRequestedState()
{
  return getInstance().requestedState;
}

void StateManager::changeToRequestedState()
{
  getInstance().prevState = getInstance().curState;
  getInstance().curState = getInstance().requestedState;
}

void StateManager::handleUserInput(String s)
{
  if (s == "o1")
  {
  }
  else if (s.indexOf("o") == 0)
  {
    getInstance().setRequestedState(STATE_OTA);
    Serial.println("Requesting OTA state");
  }
  else if (s.indexOf("r") == 0)
  {
    getInstance().setRequestedState(STATE_RUN);
    Serial.println("Requesting run state");
  }
  else if (s.indexOf("i") == 0)
  {
    getInstance().setRequestedState(STATE_IDLE);
    Serial.println("Requesting idle state");
  }
  else if (s.indexOf("k") == 0)
  {
    getInstance().setRequestedState(STATE_RESTART);
    Serial.println("Requesting restart");
  }
}

void StateManager::recvWebSerialMsg(uint8_t *data, size_t len)
{
  WebSerial.println("Received Data...");
  String d = "";
  for (int i = 0; i < len; i++)
  {
    d += char(data[i]);
  }
  WebSerial.println(d);
  handleUserInput(d);
}

void StateManager::initWebSerial()
{
  Serial.println("Initializing WebSerial");
  // WebSerial is accessible at "<IP Address>/webserial" in browser
  WebSerial.begin(getInstance().webServer);
  WebSerial.msgCallback(recvWebSerialMsg);
  getInstance().webServer->begin();
}

void StateManager::deinitWebSerial()
{
  Serial.println("Deinitializing WebSerial");
  getInstance().webServer->end();
}

void StateManager::setSlaveStates(std::map<int, JSState> s)
{
  getInstance().slaveStates = s;
}

void StateManager::setSlaveState(int id, JSState s)
{
  std::map<int, JSState> tmp = getInstance().slaveStates;
  tmp[id] = s;
  getInstance().slaveStates = tmp;
}

std::map<int, JSState> StateManager::getSlaveStates()
{
  return getInstance().slaveStates;
}

String StateManager::stateToString(JSState s)
{
  switch (s)
  {
  case STATE_INIT:
    return "STATE_INIT";
  case STATE_RUN:
    return "STATE_RUN";
  case STATE_OTA:
    return "STATE_OTA";
  case STATE_IDLE:
    return "STATE_IDLE";
  case STATE_RESTART:
    return "STATE_RESTART";
  case STATE_HANDSHAKE:
    return "STATE_HANDSHAKE";
  }
}
