
#include <AF1.h>

#ifdef ARDUINO_M5Stick_C
#include <M5StickCPlus.h> // https://github.com/m5stack/M5Stack/issues/97
#undef min
#endif

#include "state.h"
#include "stateEnt/home/home.h"
#include "stateEnt/song1/song1.h"
#include "stateEnt/song2/song2.h"
#include "img/apple.h"
#include "img/mountains.h"

void setup()
{
  Serial.setTimeout(JS_SERIAL_TIMEOUT);
  Serial.begin(JS_BAUD);
#ifdef ARDUINO_M5Stick_C
  M5.begin();
  M5.Lcd.setSwapBytes(true);
  M5.Lcd.fillScreen(TFT_WHITE);
  M5.Lcd.pushImage((DISPLAY_PORTRAIT_WIDTH - APPLE_WIDTH) / 2, (DISPLAY_PORTRAIT_HEIGHT - APPLE_HEIGHT) / 2, APPLE_WIDTH, APPLE_HEIGHT, (uint16_t *)apple);
#endif
  AF1::begin(JS_ID);
#ifdef JS_IP_A
  AF1::registerWifiAP(JSSSID, JSPASS, JS_IP_A, JS_IP_B, JS_IP_C, JS_IP_D, 192, 168, 1, 254, 255, 255, 255, 0);
#else
  AF1::registerWifiAP(JSSSID, JSPASS);
#endif
  AF1::registerStateEnt(STATE_HOME, new Home());
  AF1::registerStateEnt(STATE_SONG1, new Song1());
  AF1::registerStateEnt(STATE_SONG2, new Song2());
  AF1::registerStringHandler("song1", [](SHArg a)
                             { AF1::setRequestedState(STATE_SONG1); });
  AF1::registerStringHandler("song2", [](SHArg a)
                             { AF1::setRequestedState(STATE_SONG2); });
  AF1::registerStringHandler("v*", [](SHArg a)
                             {
                              if (AF1::getCurState() == STATE_SONG2) {
                                uint8_t v = a.getValue().toInt();
                                (static_cast<Song2 *>(AF1::getCurStateEnt()))->setValue(v);
                              } });
  AF1::registerStringHandler("h*", [](SHArg a)
                             {
                              if (AF1::getCurState() == STATE_SONG2) {
                                uint8_t h = a.getValue().toInt();
                                (static_cast<Song2 *>(AF1::getCurStateEnt()))->setHue(h);
                              } });
  AF1::registerStringHandler("s*", [](SHArg a)
                             {
                              if (AF1::getCurState() == STATE_SONG2) {
                                uint8_t s = a.getValue().toInt();
                                (static_cast<Song2 *>(AF1::getCurStateEnt()))->setSaturation(s);
                              } });
  AF1::registerStringHandler("home", [](SHArg a)
                             { AF1::setRequestedState(STATE_HOME); });
  AF1::registerStringHandler("otaws", [](SHArg a)
                             {
      DynamicJsonDocument body(1024);
      body["type"] = TYPE_CHANGE_STATE;
      body["state"] = STATE_OTA;
      AF1::getCurStateEnt()->httpPost(String("http://") + String(REMOTE_URL), body); });

#ifdef ARDUINO_M5Stick_C
  // AF1::registerStateEnt(STATE_RC2, &RC2::getInstance());
  // AF1::registerStateEnt(STATE_RC3, new RC3({SERVER_IP, "/rc/demo5/ws", SERVER_PORT, ""}));
  delay(500);
  M5.Lcd.fillScreen(TFT_WHITE);
  M5.Lcd.setRotation(0);
  M5.Lcd.pushImage(0, 0, MOUNTAINS_WIDTH, MOUNTAINS_HEIGHT, (uint16_t *)mountains);
#endif

  AF1::setInitialState(INITIAL_STATE);
  AF1::setDefaultWSClientInfo({SERVER_IP, "/lights/ws", SERVER_PORT, ""});

#ifdef VS1053_CS_PIN
  AF1::registerStringHandler("audiostop", [](SHArg a)
                             { (static_cast<LightShowBase *>(AF1::getCurStateEnt()))->stopPlaying(); });
  AF1::registerStringHandler("audiopause", [](SHArg a)
                             { (static_cast<LightShowBase *>(AF1::getCurStateEnt()))->pausePlaying(true); });
  AF1::registerStringHandler("audioresume", [](SHArg a)
                             { (static_cast<LightShowBase *>(AF1::getCurStateEnt()))->pausePlaying(false); });
#endif
}

void loop()
{
  AF1::update();
}
