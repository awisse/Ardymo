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
  Pt pt(30, 0), offset(64, 32), rotated, target;
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

  rotate(&pt, &rotated, alpha);
  target = offset + rotated;
  Platform::drawLine(offset.x, offset.y, 
      target.x, target.y, COLOUR_WHITE);
  for (i=0; i<ARROW_LENGTH; i++) {
  }
}

/*************** Rotation matrix *******************
 * | cos(\theta) -sin(\theta) |
 * | sin(\theta)  cos(\theta) |
 *
 * We need only two float values per angle: cos(\theta) and sin(\theta)
*/

void rotate(Pt *from, Pt *to, int16_t angle) {
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


