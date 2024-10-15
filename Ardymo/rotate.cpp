//
// Define sin and cos as a function of degrees in 1 degree (positive integer)
// steps
#include "platform.h"
#include "rotate.h"

rotn get_rotn(int16_t alpha) {
  // Get the cosinus and the sinus of the angle alpha from the table
  // in rotate.h
  rotn rot_vect;
  float x;

  alpha %= 360;
  if (alpha < 0) {
    alpha += 360;
  }

  if (alpha <= 45) {
    rot_vect.cos = pgm_read_float(&Omega[alpha].cos);
    rot_vect.sin = pgm_read_float(&Omega[alpha].sin);
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


