// vim: ts=2:sts=2:expandtab

#include "model.h"
#include "platform.h"
#include "matrix.h"
#include "structs.h"
#include "defines.h"

void rotate(point *from, point *to, int16_t angle);
rotn get_rotn(int16_t angle);

void initialize() {}

void step_model(uint16_t frame) {

  // point p, p0, center;
  // int16_t x, y; // Coordinates to be drawn
  // uint16_t step = frame % (2 * ANGLES - 1);

  // center = { SCREEN_WIDTH / 2, 0 };
  // p0 = { SCREEN_HEIGHT - 1, 0 };

  // p = rotate(&p0, step);

  // x = center.x + p.x;
  // y = center.y + p.y;

  // Platform::setCursor(0, 0);
  // Platform::print("p(x, y) = ");
  // Platform::print("(");
  // Platform::print(p.x);
  // Platform::print(", ");
  // Platform::print(p.y);
  // Platform::println(")");
  // Platform::print("step    = ");
  // Platform::println(step);

  // Platform::drawPixel(x, y);

  static uint16_t alpha = 0;
  point pt = {30, 0}, offset = {64, 32}, rpt;
  int i;

  /* Platform::pollButtons(); */
  if (Platform::pressed(INPUT_LEFT)) {
    alpha = alpha ? alpha - 3 : 357;
  } else if (Platform::pressed(INPUT_RIGHT)) {
    alpha = (alpha + 3) % 360;
  } else {
    return;
  }

  Platform::clear();

  Platform::setCursor(0, 0);
  Platform::print("alpha: ");
  Platform::print(alpha);

  rotate(&pt, &rpt, alpha);
  Platform::drawLine(offset.x, offset.y, offset.x + rpt.x,
      offset.y + rpt.y, COLOUR_WHITE);

}

/*************** Rotation matrix *******************
 * | cos(\theta) -sin(\theta) |
 * | sin(\theta)  cos(\theta) |
 *
 * We need only two float values per angle: cos(\theta) and sin(\theta)
*/

void rotate(point *from, point *to, int16_t angle) {
  /* Rotate point `from` by angle degrees around the center of the screen.
   * Return the rotated point in *to.
   * Positive angle between 0 and 359 degrees.
   */
  rotn  r = get_rotn(angle);

  // Matrix multiplication
  to->x = r.cos * from->x - r.sin * from->y;
  to->y = r.sin * from->x + r.cos * from->y;
}

rotn get_rotn(int16_t alpha) {
  // Get the correct element of the rotation matrix omega.
  rotn rot_vect;
  float x;

  alpha = alpha % 360;
  if (alpha < 0) {
    alpha = 360 + alpha;
  }

  if (alpha <= 45) {
    rot_vect.cos = pgm_read_float(&omega[alpha].cos);
    rot_vect.sin = pgm_read_float(&omega[alpha].sin);
    return rot_vect;
  }

  if (alpha > 180)
  {
    rot_vect = get_rotn(360 - alpha);
    rot_vect.sin = -rot_vect.sin;
  }
  else if (alpha > 90)
  {
    rot_vect = get_rotn(180 - alpha);
    rot_vect.cos = -rot_vect.cos;
  }
  else {
    rot_vect = get_rotn(90 - alpha);
    x = rot_vect.cos;
    rot_vect.cos = rot_vect.sin;
    rot_vect.sin = x;
  }

  return rot_vect;
}


