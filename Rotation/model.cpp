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


  static bool done = false;
	float x, y, z, xzz, zz;
  uint32_t *ix, *iy, *iz, *izz, *ixzz;

  x = 2.;
  y = 0.34;
  z = 2.34;
  zz = z - (uint32_t)z;
  xzz = x + zz;
  ix = (uint32_t*)&x;
  iy = (uint32_t*)&y;
  iz = (uint32_t*)&z;
  izz = (uint32_t*)&zz;
  ixzz = (uint32_t*)&xzz;


  if (done) {
    return;
  }

  Platform::setCursor(0, 0);

  // x
  Platform::print("x   ");
  Platform::print(x, 4);
  Platform::print(" ");
  Platform::println(*ix, HEX);
  // y
  Platform::print("y   ");
  Platform::print(y, 4);
  Platform::print(" ");
  Platform::println(*iy, HEX);
  // z
  Platform::print("z   ");
  Platform::print(z, 4);
  Platform::print(" ");
  Platform::println(*iz, HEX);
  // zz
  Platform::print("zz  ");
  Platform::print(zz, 4);
  Platform::print(" ");
  Platform::println(*izz, HEX);
  // xzz
  Platform::print("xzz ");
  Platform::print(xzz, 4);
  Platform::print(" ");
  Platform::println(*ixzz, HEX);

  done = true;
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

