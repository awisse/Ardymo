/* vim: ft=cpp:ts=2:sts=2:expandtab
 *
 * Struct and classes used in program
 */
#include "objects.h"
#include "matrix.h"

rotn get_rotn(int16_t angle);

Vec Vec::operator+=(const Vec& p) {
  this->x += p.x;
  this->y += p.y;
  return *this;
}

Vec Vec::operator+(const Vec& p) {
  Vec newpt = p;
  newpt += *this;
  return newpt;
}

Vec Vec::operator+(const point p) {
  Vec newpt = Vec(p);
  newpt += *this;
  return newpt;
}

Vec Vec::operator-() {
  Vec newpt = Vec(-x, -y);
  return newpt;
}

Vec Vec::operator-(const Vec& p) {
  Vec newpt = *this + (-Vec(p));
  return newpt;
}

Vec Vec::operator*(float a) {
  Vec newpt = *this;
  newpt.x *= a;
  newpt.y *= a;
  return newpt;
}

float Vec::operator*(const Vec& p) {
  return p.x * x + p.y * y;
}

bool Vec::operator==(const Vec& p) {
  return (x == p.x) && (y == p.y);

}

point Vec::as_point(void) {
  point p = {this->x, this->y};
  return p;
}

float Vec::sq(void) {
  return x*x + y*y;
}

/*************** Rotation matrix (clockwise)  *******************
 * |  cos(\theta)  sin(\theta) |
 * | -sin(\theta)  cos(\theta) |
 *
 * We need only two float values per angle: cos(\theta) and sin(\theta)
 * The parameter \theta is an integer as we only have sines and cosines for
 * integer multiples between 0 and 359.
*/
Vec Vec::rotate(int16_t theta) {

  if ((theta < 0) || (theta > 359)) {
    return Vec(*this);
  }
  rotn  rot = get_rotn(theta);

  // Matrix multiplication
  return Vec(rot.cos * x - rot.sin * y, rot.sin * x + rot.cos * y);
}

rotn get_rotn(int16_t alpha) {
  // Get the cosinus and the sinus of the angle alpha from the table
  // in matrix.h
  rotn rot_vect;
  float x;

  alpha = alpha % 360;
  if (alpha < 0) {
    alpha = 360 + alpha;
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


