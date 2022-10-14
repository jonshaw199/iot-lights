#ifndef STATEENT_RC2_H_
#define STATEENT_RC2_H_

#include <AF1.h>

#include <M5StickCPlus.h>
#undef min // https://github.com/m5stack/M5Stack/issues/97

#include "state.h"

typedef struct
{
  double x;
  double y;
} point_2d_t;

typedef struct
{
  double x;
  double y;
  double z;
} point_3d_t;

typedef struct
{
  point_3d_t start_point;
  point_3d_t end_point;
} line_3d_t;

class RC2 : public Base
{
public:
  static RC2 &getInstance();
  void setup();
  void loopM5();

protected:
  static float accX;
  static float accY;
  static float accZ;
  static double theta;
  static double last_theta;
  static double phi;
  static double last_phi;
  static double alpha;
  static line_3d_t x;
  static line_3d_t y;
  static line_3d_t z;
  static line_3d_t rect_source[12];
  static line_3d_t rect_dis;

private:
  RC2();
  double r_rand = PI / 180;
  double r_alpha = 19.47 * PI / 180;
  double r_gamma = 20.7 * PI / 180;
  double sin_alpha = sin(19.47 * PI / 180);
  double cos_alpha = cos(19.47 * PI / 180);
  double sin_gamma = sin(20.7 * PI / 180);
  double cos_gamma = cos(20.7 * PI / 180);
  bool TestMode = false;

  line_3d_t rect[12] = {
      {.start_point = {-1, -1, 1}, .end_point = {1, -1, 1}},
      {.start_point = {1, -1, 1}, .end_point = {1, 1, 1}},
      {.start_point = {1, 1, 1}, .end_point = {-1, 1, 1}},
      {.start_point = {-1, 1, 1}, .end_point = {-1, -1, 1}},
      {
          .start_point = {-1, -1, 1},
          .end_point = {-1, -1, -1},
      },
      {
          .start_point = {1, -1, 1},
          .end_point = {1, -1, -1},
      },
      {
          .start_point = {1, 1, 1},
          .end_point = {1, 1, -1},
      },
      {
          .start_point = {-1, 1, 1},
          .end_point = {-1, 1, -1},
      },
      {.start_point = {-1, -1, -1}, .end_point = {1, -1, -1}},
      {.start_point = {1, -1, -1}, .end_point = {1, 1, -1}},
      {.start_point = {1, 1, -1}, .end_point = {-1, 1, -1}},
      {.start_point = {-1, 1, -1}, .end_point = {-1, -1, -1}},
  };

  void setupM5();
  bool point3Dto2D(point_3d_t *source, point_2d_t *point);
  bool point2DToDisPoint(point_2d_t *point, uint8_t *x, uint8_t *y);
  bool printLine3D(TFT_eSprite *display, line_3d_t *line, uint32_t color);
  void RotatePoint(point_3d_t *point, double x, double y, double z);
  void RotatePoint(point_3d_t *point, point_3d_t *point_new, double x, double y, double z);
  void Displaybuff();
};

#endif
