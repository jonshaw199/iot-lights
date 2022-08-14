
#ifndef STATEMANAGER_STATEMANAGER_H_
#define STATEMANAGER_STATEMANAGER_H_

#include <queue>
#include <map>
#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSerial.h>

#include "state/state.h"
#include "message/message.h"

class StateManager
{
  StateManager(); // constructor
  JSState curState;
  JSState prevState;
  JSState requestedState;
  AsyncWebServer *webServer;
  // Master only
  std::map<int, JSState> slaveStates;

protected:
  static void recvWebSerialMsg(uint8_t *data, size_t len);

public:
  static StateManager &getInstance();
  static JSState getCurState();
  static void init(JSState s);
  static JSState getPrevState();
  static void setRequestedState(JSState s);
  static JSState getRequestedState();
  static void changeToRequestedState();
  static void handleUserInput(String s);
  static void initWebSerial();
  static void deinitWebSerial();
  static String stateToString(JSState s);
  // Master only
  static void setSlaveStates(std::map<int, JSState> s);
  static void setSlaveState(int id, JSState s);
  static std::map<int, JSState> getSlaveStates();
  static void setBuiltinLED(bool on);
};

#endif // STATEMANAGER_STATEMANAGER_H_