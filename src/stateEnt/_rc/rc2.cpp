#ifdef ARDUINO_M5Stick_C

#include "rc2.h"
#include "img/space.h"
#include "state.h"

float RC2::accX;
float RC2::accY;
float RC2::accZ;
double RC2::theta;
double RC2::last_theta;
double RC2::phi;
double RC2::last_phi;
double RC2::alpha;
line_3d_t RC2::x;
line_3d_t RC2::y;
line_3d_t RC2::z;
line_3d_t RC2::rect_source[12];
line_3d_t RC2::rect_dis;

TFT_eSprite Disbuff = TFT_eSprite(&M5.Lcd);

RC2::RC2()
{
  intervalEventMap["RC2_1"] = IntervalEvent("RC2_1", 50, [](IECBArg a)
                                            { getInstance().loopM5(); });

  /*intervalEventMap.insert(std::pair<String, IntervalEvent>("RC2_2", IntervalEvent(200, [](IECBArg a)
                                                                                  {
    DynamicJsonDocument d(1024);

    // Required
    d["state"] = STATE_RC2;
    d["type"] = TYPE_RC_DATA;

    d["accX"] = accX;
    d["accY"] = accY;
    d["accZ"] = accZ;
    d["theta"] = theta;
    d["lastTheta"] = last_theta;
    d["phi"] = phi;
    d["lastPhi"] = phi;
    d["alpha"] = alpha;
    d["x"]["startPoint"]["x"] = x.start_point.x;
    d["x"]["startPoint"]["y"] = x.start_point.y;
    d["x"]["startPoint"]["z"] = x.start_point.z;
    d["x"]["endPoint"]["x"] = x.end_point.x;
    d["x"]["endPoint"]["y"] = x.end_point.y;
    d["x"]["endPoint"]["z"] = x.end_point.z;
    d["y"]["startPoint"]["x"] = y.start_point.x;
    d["y"]["startPoint"]["y"] = y.start_point.y;
    d["y"]["startPoint"]["z"] = y.start_point.z;
    d["y"]["endPoint"]["x"] = y.end_point.x;
    d["y"]["endPoint"]["y"] = y.end_point.y;
    d["y"]["endPoint"]["z"] = y.end_point.z;
    d["z"]["startPoint"]["x"] = z.start_point.x;
    d["z"]["startPoint"]["y"] = z.start_point.y;
    d["z"]["startPoint"]["z"] = z.start_point.z;
    d["z"]["endPoint"]["x"] = z.end_point.x;
    d["z"]["endPoint"]["y"] = z.end_point.y;
    d["z"]["endPoint"]["z"] = z.end_point.z;

    pushOutbox(d); })));*/
}

RC2 &RC2::getInstance()
{
  static RC2 instance; // Guaranteed to be destroyed.
                       // Instantiated on first use.
  return instance;
}

void RC2::setup()
{
  LightShowBase::setup();
  setupM5();
}

void RC2::setupM5()
{
  M5.Lcd.setRotation(3);

  Disbuff.setSwapBytes(true);
  Disbuff.setTextColor(TFT_WHITE);
  Disbuff.createSprite(240, 135);
  Disbuff.fillRect(0, 0, 240, 135, Disbuff.color565(10, 10, 10));
  Disbuff.pushSprite(0, 0);

  M5.update();
  M5.Axp.ScreenBreath(80);
  M5.Imu.Init();

  accX = 0;
  accY = 0;
  accZ = 0;

  theta = 0;
  last_theta = 0;
  phi = 0;
  last_phi = 0;
  alpha = 0.2;

  x = {.start_point = {0, 0, 0}, .end_point = {0, 0, 0}};
  y = {.start_point = {0, 0, 0}, .end_point = {0, 0, 0}};
  z = {.start_point = {0, 0, 0}, .end_point = {0, 0, 30}};

  for (int n = 0; n < 12; n++)
  {
    rect_source[n].start_point.x = rect[n].start_point.x * 30;
    rect_source[n].start_point.y = rect[n].start_point.y * 30;
    rect_source[n].start_point.z = rect[n].start_point.z * 30;
    rect_source[n].end_point.x = rect[n].end_point.x * 30;
    rect_source[n].end_point.y = rect[n].end_point.y * 30;
    rect_source[n].end_point.z = rect[n].end_point.z * 30;
  }
}

void RC2::loopM5()
{
  if /* while */ ((!M5.BtnA.isPressed()) && (!M5.BtnB.isPressed()))
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

    // Disbuff.fillRect(0, 0, 240, 135, TFT_BLACK);
    Disbuff.pushImage(0, 0, SPACE_WIDTH, SPACE_HEIGHT, space);

    // Disbuff.setRotation(0);
    Disbuff.setTextSize(1);
    Disbuff.setCursor(10, 115);
    Disbuff.printf("%.2f", theta);
    Disbuff.setCursor(10, 125);
    Disbuff.printf("%.2f", phi);
    // Displaybuff();
    // delay(20); JS

    z.end_point.x = 0;
    z.end_point.y = 0;
    z.end_point.z = 60;
    RotatePoint(&z.end_point, theta, phi, 0);
    RotatePoint(&z.end_point, &x.end_point, -90, 0, 0);
    RotatePoint(&z.end_point, &y.end_point, 0, 90, 0);

    for (int n = 0; n < 12; n++)
    {
      RotatePoint(&rect_source[n].start_point, &rect_dis.start_point,
                  theta, phi, (double)0);
      RotatePoint(&rect_source[n].end_point, &rect_dis.end_point, theta,
                  phi, (double)0);
      printLine3D(&Disbuff, &rect_dis, TFT_WHITE);
    }
    printLine3D(&Disbuff, &x, TFT_RED);
    printLine3D(&Disbuff, &y, TFT_GREEN);
    printLine3D(&Disbuff, &z, TFT_BLUE);
    Displaybuff();
    last_theta = theta;
    last_phi = phi;

    M5.update();
  }
  // delay(50); JS
}

bool RC2::point3Dto2D(point_3d_t *source, point_2d_t *point)
{
  point->x = (source->x * cos_gamma) - (source->y * sin_gamma);
  point->y = -(source->x * sin_gamma * sin_alpha) -
             (source->y * cos_gamma * sin_alpha) + (source->z * cos_alpha);
  return true;
}

bool RC2::point2DToDisPoint(point_2d_t *point, uint8_t *x, uint8_t *y)
{
  *x = point->x + 120;
  *y = 67 - point->y;
  return true;
}

bool RC2::printLine3D(TFT_eSprite *display, line_3d_t *line, uint32_t color)
{
  uint8_t start_x, start_y, end_x, end_y;
  point_2d_t point;
  point3Dto2D(&line->start_point, &point);
  point2DToDisPoint(&point, &start_x, &start_y);
  point3Dto2D(&line->end_point, &point);
  point2DToDisPoint(&point, &end_x, &end_y);

  display->drawLine(start_x, start_y, end_x, end_y, color);

  return true;
}

void RC2::RotatePoint(point_3d_t *point, double x, double y, double z)
{
  if (x != 0)
  {
    point->y = point->y * cos(x * r_rand) - point->z * sin(x * r_rand);
    point->z = point->y * sin(x * r_rand) + point->z * cos(x * r_rand);
  }

  if (y != 0)
  {
    point->x = point->z * sin(y * r_rand) + point->x * cos(y * r_rand);
    point->z = point->z * cos(y * r_rand) - point->x * sin(y * r_rand);
  }

  if (z != 0)
  {
    point->x = point->x * cos(z * r_rand) - point->y * sin(z * r_rand);
    point->y = point->x * sin(z * r_rand) + point->y * cos(z * r_rand);
  }
}

void RC2::RotatePoint(point_3d_t *point, point_3d_t *point_new, double x, double y,
                      double z)
{
  if (x != 0)
  {
    point_new->y = point->y * cos(x * r_rand) - point->z * sin(x * r_rand);
    point_new->z = point->y * sin(x * r_rand) + point->z * cos(x * r_rand);
  }

  if (y != 0)
  {
    point_new->x = point->z * sin(y * r_rand) + point->x * cos(y * r_rand);
    point_new->z = point->z * cos(y * r_rand) - point->x * sin(y * r_rand);
  }

  if (z != 0)
  {
    point_new->x = point->x * cos(z * r_rand) - point->y * sin(z * r_rand);
    point_new->y = point->x * sin(z * r_rand) + point->y * cos(z * r_rand);
  }
}

void RC2::Displaybuff()
{
  Disbuff.pushSprite(0, 0);
}

#endif