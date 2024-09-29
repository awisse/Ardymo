// vim: ts=2:sts=2:expandtab

#include "model.h"
#include "platform.h"
#include "matrix.h"
#include "structs.h"
#include "shapes.h"
#include "defines.h"

void rotate(Pt *from, Pt *to, int16_t angle);
rotn get_rotn(int16_t angle);

void initialize() {}

void step_model(uint16_t frame) {

  static uint16_t alpha = 0;
  Pt start(-30, 0), end(30, 0), offset(64, 32);
  Pt rotated, start_rotated, end_rotated;
  int i;

  /* Platform::pollButtons(); */
  if (Platform::pressed(INPUT_LEFT)) {
    alpha = alpha ? alpha - 3 : 357;
  } else if (Platform::pressed(INPUT_RIGHT)) {
    alpha = (alpha + 3) % 360;
  } else if (rotated.x) { // Run at least once
    return;
  }

  Platform::clear();

  Platform::setCursor(0, 0);
  Platform::print("alpha: ");
  Platform::print(alpha);

  rotate(&start, &start_rotated, alpha);
  start_rotated = offset + start_rotated;
  rotate(&end, &end_rotated, alpha);
  end_rotated = offset + end_rotated;
  Platform::drawLine(start_rotated.x, start_rotated.y,
      end_rotated.x, end_rotated.y, COLOUR_WHITE);
}

/*************** Rotation matrix *******************
 * | cos(\theta) -sin(\theta) |
 * | sin(\theta)  cos(\theta) |
 *
 * We need only two float values per angle: cos(\theta) and sin(\theta)
*/

void rotate(Pt *from, Pt *to, int16_t theta) {
  /* Rotate point `from` by angle degrees clockwise around the center of the
   * screen.
   * Return the rotated point in *to.
   * Positive angle `theta` between 0 and 359 degrees.
   */

  if ((theta < 0) || (theta > 359)) {
    *to=*from;
    Platform::setCursor(0, 0);
    Platform::print("rotate: theta!!");
    return;
  }
  rotn  rot = get_rotn(theta);

  // Matrix multiplication
  to->x = rot.cos * from->x - rot.sin * from->y;
  to->y = rot.sin * from->x + rot.cos * from->y;
}

rotn get_rotn(int16_t alpha) {
  // Get the cosinus and the sinus of the angle alpha.
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


