
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
  curState = STATE_INIT;
  requestedState = curState;
  webServer = new AsyncWebServer(80);
}

void StateManager::queueMsg(js_message msg)
{
  getInstance().msgQueue.push(msg);
}

// Gets/returns copy of front and also pops
js_message StateManager::dequeMsg()
{
  js_message msg = getInstance().msgQueue.front();
  getInstance().msgQueue.pop();
  return msg;
}

// Read only reference
const std::queue<js_message> &StateManager::getMsgQueue()
{
  return getInstance().msgQueue;
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
  if (s.indexOf("o") == 0)
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