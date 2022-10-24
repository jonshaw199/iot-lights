#ifdef ARDUINO_M5Stick_C

#include <AF1.h>

#include <M5StickCPlus.h>
#undef min // https://github.com/m5stack/M5Stack/issues/97

#include "rc3.h"
#include "state.h"
#include "img/sponge.h"

float RC3::accX = 0;
float RC3::accY = 0;
float RC3::accZ = 0;
double RC3::theta = 0;
double RC3::last_theta = 0;
double RC3::phi = 0;
double RC3::last_phi = 0;
double RC3::alpha = 0.2;

RC3::RC3(ws_client_info i)
{
  setWSClientInfo(i);

  intervalEventMap["RC3_1"] = IntervalEvent("RC3_1", 50, [](IECBArg a)
                                            {
                                              M5.Imu.getAccelData(&accX, &accY, &accZ);
                                              // M5.MPU6886.getAccelData(&accX, &accY, &accZ);
                                              if ((accX < 1) && (accX > -1))
                                              {
                                                theta = asin(-accX) * 57.295;
                                              }
                                              if (accZ != 0)
                                              {
                                                phi = atan(accY / accZ) * 57.295;
                                              }

                                              theta = alpha * theta + (1 - alpha) * last_theta;
                                              phi = alpha * phi + (1 - alpha) * last_phi;

                                              // Do stuff
                                              M5.Lcd.fillRect(0, DISPLAY_PORTRAIT_HEIGHT - 33, DISPLAY_PORTRAIT_WIDTH, 33, WHITE);
                                              M5.Lcd.setTextSize(1);
                                              M5.Lcd.setTextColor(RED);
                                              M5.Lcd.setCursor(5, DISPLAY_PORTRAIT_HEIGHT - 20);
                                              M5.Lcd.printf("%.2f", theta);
                                              M5.Lcd.setCursor(5, DISPLAY_PORTRAIT_HEIGHT - 10);
                                              M5.Lcd.printf("%.2f", phi);
                                              M5.update();

                                              last_theta = theta;
                                              last_phi = phi; });

  intervalEventMap["RC3_2"] = IntervalEvent("RC3_2", 200, [](IECBArg a)
                                            {
                                              DynamicJsonDocument d(1024);

                                              // Required
                                              d["state"] = 4;
                                              d["type"] = TYPE_RC_DATA;

                                              d["accX"] = accX;
                                              d["accY"] = accY;
                                              d["accZ"] = accZ;
                                              d["theta"] = theta;
                                              d["lastTheta"] = last_theta;
                                              d["phi"] = phi;
                                              d["lastPhi"] = phi;
                                              d["alpha"] = alpha;

                                              pushOutbox(d); });
}

void RC3::setup()
{
  LightShowBase::setup();
  M5.Lcd.fillScreen(TFT_WHITE);
  M5.Lcd.setRotation(0);
  M5.Lcd.pushImage((DISPLAY_PORTRAIT_WIDTH - SPONGE_WIDTH) / 2, (DISPLAY_PORTRAIT_HEIGHT - SPONGE_HEIGHT) / 2, SPONGE_WIDTH, SPONGE_HEIGHT, (uint16_t *)sponge);

  DynamicJsonDocument body(1024);
  body["type"] = TYPE_CHANGE_STATE;
  body["state"] = 4;
  httpPost(String("http://") + String(REMOTE_URL), body);
}

void RC3::preStateChange(int s)
{
  DynamicJsonDocument body(1024);
  body["type"] = TYPE_CHANGE_STATE;
  body["state"] = STATE_IDLE_BASE;
  httpPost(String("http://") + String(REMOTE_URL), body);
}

#endif
