/* vim: ft=cpp:ts=2:sts=2:expandtab
 *
 * Struct and classes used in program
 */
#include "objects.h"
#include "rotate.h"
#include "math.h"

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

float Vec::det(const Vec& p) {
  return x * p.y - y * p.x;
}

float Vec::div(const Vec& p) {
  if (fabsf(det(p)) > epsilon) {
    // this and p are not collinear. Division doesn't make sense
    return 0.0 / 0.0; // NaN
  }
  return (fabsf(x)>fabsf(y)) ? x / p.x : y / p.y;
}

float Vec::sq(void) {
  return x*x + y*y;
}

float Vec::distance(const Vec& p) {
  /* return sqrtf((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y)); */
  return sqrtf((*this - p).sq());
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
  rotn rot = get_rotn(theta);

  // Matrix multiplication
  return Vec(rot.cos * x - rot.sin * y, rot.sin * x + rot.cos * y);
}
