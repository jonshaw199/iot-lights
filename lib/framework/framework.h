#ifndef FRAMEWORK_FRAMEWORK_H_
#define FRAMEWORK_FRAMEWORK_H_

#include "stateent/base/base.h"

#include "messageHandler/messageHandler.h"
#include "stateManager/stateManager.h"

class Framework
{
public:
  void setup();
  void loop();
  void registerStateEnt(int i, Base *s);
  void registerStringHandler(String s, string_input_handler h);
};

#endif // FRAMEWORK_FRAMEWORK_H_