#include "lightShowBase.h"
#include "state.h"

static bool motion = false;

void LightShowBase::setup()
{
  Base::setup();
#ifdef IR_SENSOR_PIN
  pinMode(IR_SENSOR_PIN, INPUT); // sensor pin INPUT
#endif
}

void LightShowBase::loop()
{
  Base::loop();
#ifdef IR_SENSOR_PIN
  if (digitalRead(IR_SENSOR_PIN) == 1)
  {
    // Motion
    if (!motion)
    {
      motion = true;
      Serial.println("Motion begin");
      AF1Msg msg;
      msg.setState(StateManager::getCurState());
      msg.setType(TYPE_MOTION);
      msg.getJson()["motion"] = true;
      msg.setRecipients({255});
      pushOutbox(msg);
    }
  }
  else
  {
    if (motion)
    {
      motion = false;
      Serial.println("Motion end");
      AF1Msg msg;
      msg.setState(StateManager::getCurState());
      msg.setType(TYPE_MOTION);
      msg.getJson()["motion"] = false;
      msg.setRecipients({255});
      pushOutbox(msg);
    }
  }
#endif
}