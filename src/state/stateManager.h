
#ifndef STATE_STATEMANAGER_H_
#define STATE_STATEMANAGER_H_

#include <queue>
#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSerial.h>

#include "state.h"
#include "message/message.h"

class StateManager
{
  StateManager(); // constructor
  JSState curState;
  JSState prevState;
  std::queue<js_message> msgQueue;
  JSState requestedState;
  AsyncWebServer *webServer;

protected:
  static void recvWebSerialMsg(uint8_t *data, size_t len);

public:
  static StateManager &getInstance();
  static JSState getCurState();
  // Removed to force "requesting" state change
  // void setCurState(JSState s);
  static JSState getPrevState();
  static void queueMsg(js_message msg);
  static js_message dequeMsg();
  static const std::queue<js_message> &getMsgQueue();
  static void setRequestedState(JSState s);
  static JSState getRequestedState();
  static void changeToRequestedState();
  static void handleUserInput(String s);
  static void initWebSerial();
  static void deinitWebSerial();
};

#endif // STATE_STATEMANAGER_H_