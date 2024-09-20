// vim: ts=2:sts=2:expandtab

#include "model.h"
#include "platform.h"
#include "matrix.h"
#include "defines.h"

point rotate(point *p0, uint8_t step);

void init_game() {}

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

  int16_t counter;
  static bool done = false;
  uint8_t bases[] = {DEC, HEX, OCT, BINARY};

  if (done) {
    return;
  }

  Platform::setCursor(0, 0);
  for (counter=0; counter<4; counter++) {
    Platform::print((uint16_t)0xabcd, bases[counter]);
    Platform::setCursor(64, Platform::getCursorY());
    Platform::println((int16_t)0xabcd, bases[counter]);
  }
  Platform::print(-21555, HEX);

}

/*************** Rotation matrix *******************
 * | cos(\theta) -sin(\theta) |
 * | sin(\theta)  cos(\theta) |
 *
 * We need only two float values per angle: cos(\theta) and sin(\theta)
*/

point rotate(point *p0, uint8_t step) {
  /* Rotate point p0 by step * 6  degrees around the center of the screen.
   * Return the rotated point.
   * Between 0 and Pi/2 for now.
   */
  point p = {0, 0};
  rotn  r;
  float cos, sin; // Dependent on step.

  if (step < ANGLES) {
    r = omega[step];
    cos = r.cos;
    sin = r.sin;
  }  else {
    r = omega[2 * (ANGLES - 1) - step];
    cos = -r.cos;
    sin = r.sin;
  }

  // Matrix multiplication
  p.x = cos * p0->x - sin * p0->y;
  p.y = sin * p0->x + cos * p0->y;

  return p;
}

