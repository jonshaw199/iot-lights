#ifndef FRAMEWORK_FRAMEWORK_H_
#define FRAMEWORK_FRAMEWORK_H_

#include "stateent/base/base.h"

#include "messageHandler/messageHandler.h"
#include "stateManager/stateManager.h"

class Framework
{
public:
  static void setup();
  static void loop();
  static void registerStateEnt(int i, Base *s, String n);
  static void registerStringHandler(String s, string_input_handler h);
};

#endif // FRAMEWORK_FRAMEWORK_H_