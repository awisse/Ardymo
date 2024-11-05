/* vim: ft=cpp:ts=2:sts=2:expandtab
 *
 * The `Vec` class used in the program.
 */
#include "objects.h"
#include "rotate.h"
#include "math.h"

Vec Vec::operator+=(const Vec& p) {
  this->x += p.x;
  this->y += p.y;
  return *this;
}

Vec Vec::operator+(const Vec& p) const {
  Vec newpt = p;
  newpt += *this;
  return newpt;
}

Vec Vec::operator+(const point p) const {
  Vec newpt = Vec(p);
  newpt += *this;
  return newpt;
}

Vec Vec::operator-(void) const {
  Vec newpt = Vec(-x, -y);
  return newpt;
}

Vec Vec::operator-(const Vec& p) const {
  Vec newpt = *this + (-Vec(p));
  return newpt;
}

Vec Vec::operator*(float a) const {
  Vec newpt = *this;
  newpt.x *= a;
  newpt.y *= a;
  return newpt;
}

Vec Vec::operator/(float a) const {
  // No room for checking zero division. Live with infinity!
  Vec newpt = *this;
  newpt.x /= a;
  newpt.y /= a;
  return newpt;
}

float Vec::operator*(const Vec& p) const {
  return p.x * x + p.y * y;
}

bool Vec::operator==(const Vec& p) const {
  return (x == p.x) && (y == p.y);

}

point Vec::as_point(void) const {
  point p = {this->x, this->y};
  return p;
}

float Vec::det(const Vec& p) const {
  return x * p.y - y * p.x;
}

float Vec::div(const Vec& p) const {
  if (fabsf(det(p)) > epsilon) {
    // this and p are not collinear. Division doesn't make sense
    return 0.0 / 0.0; // NaN
  }
  return (fabsf(x)>fabsf(y)) ? x / p.x : y / p.y;
}

float Vec::square(void) const {
  return x*x + y*y;
}

float Vec::length(void) const {
  return sqrtf(this->square());
}

float Vec::distance(const Vec& p) const {
  /* return sqrtf((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y)); */
  Vec p0 = *this; // needed for const
  return sqrtf((p0 - p).square());
}

Vec Vec::normalized(void) const {
  float l = this->length();
  Vec p0 = *this; // needed for const
  return p0 / l;
}

void Vec::flipX(void) {
  this->x = -this->x;
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

  rotn rot = get_rotn(theta);

  // Matrix multiplication
  return Vec(rot.cos * x - rot.sin * y, rot.sin * x + rot.cos * y);
}
