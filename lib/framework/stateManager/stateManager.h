
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
#include "stateent/base/base.h"

typedef void (*string_input_handler)();

class StateManager
{
  StateManager(); // constructor
  int curState;
  int prevState;
  int requestedState;
  AsyncWebServer *webServer;
  Base *stateEnt;
  std::map<int, Base *> stateEntMap;
  std::map<String, string_input_handler> stringHandlerMap;

protected:
  static void recvWebSerialMsg(uint8_t *data, size_t len);

public:
  static StateManager &getInstance();
  static int getCurState();
  static int getPrevState();
  static void setRequestedState(int s);
  static int getRequestedState();
  static void changeToRequestedState();
  static void handleUserInput(String s);
  static void initWebSerial();
  static void deinitWebSerial();
  static String stateToString(int s);
  static void setBuiltinLED(bool on);
  static bool handleStateChange(int s);
  static void init();
  static void loop();
  static void registerStateEnt(int i, Base *s);
  static void registerStringHandler(String s, string_input_handler h);
};

#endif // STATEMANAGER_STATEMANAGER_H_
