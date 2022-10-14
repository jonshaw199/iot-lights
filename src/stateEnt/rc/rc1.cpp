#include <AF1.h>

#include <M5StickCPlus.h>
#undef min // https://github.com/m5stack/M5Stack/issues/97

#include "state.h"
#include "img/mountains.h"

class RC1 : public Base
{
  int lastState;

public:
  void setup()
  {
    Base::setup();

    lastState = 0;

    M5.Lcd.fillScreen(TFT_WHITE);
    M5.Lcd.setRotation(0);
    M5.Lcd.pushImage(0, 0, MOUNTAINS_WIDTH, MOUNTAINS_HEIGHT, (uint16_t *)mountains);
  }

  void loop()
  {
    Base::loop();

    M5.update(); // Read the press state of the key.  读取按键 A, B, C 的状态
    if (M5.BtnA.wasReleased())
    { // If the button A is pressed.  如果按键 A 被按下
      M5.Lcd.print('A');
      DynamicJsonDocument body(1024);
      body["type"] = TYPE_CHANGE_STATE;
      lastState = lastState == 0 ? 2 : 0;
      body["state"] = lastState;
      httpPost(String("http://") + String(STRINGIFY(REMOTE_URL)), body);
    }
    /*else if (M5.BtnB.wasReleased())
    { // If the button B is pressed. 如果按键
      // B 被按下，
      // M5.Lcd.print('B');
      StateManager::setRequestedState(STATE_RC2);
    }*/
    /*else if (M5.BtnB.wasReleasefor(
                 700))
    { // The button B is pressed for 700ms. 按键 B 按下
      // 700ms,屏幕清空
      M5.Lcd.fillScreen(
          BLACK); // Set BLACK to the background color.  将黑色设置为底色
      M5.Lcd.setCursor(0, 0);
    }*/
  }

  String getName()
  {
    return "STATE_RC1";
  }
};
