/* vim: ft=cpp:ts=2:sts=2:expandtab
 *
 * Struct and classes used in program
 */
#include "pt.h"



Pt Pt::operator+=(const Pt& p) {
  this->x += p.x;
  this->y += p.y;
  return *this;
}
Pt Pt::operator+(const Pt& p) {
  Pt newpt = p;
  newpt.x = this->x + p.x;
  newpt.y = this->y + p.y;
  return newpt;
}

Pt Pt::operator-() {
  Pt newpt = Pt(-x, -y);
  return newpt;
}

Pt Pt::operator-(const Pt& p) {
  Pt newpt = *this + (-Pt(p));
  return newpt;
}

Pt Pt::operator*(float a) {
  Pt newpt = *this;
  newpt.x *= a;
  newpt.y *= a;
  return newpt;
}

bool Pt::operator==(const Pt& p) {
  return (this->x == p.x) && (this->y == p.y);

}

