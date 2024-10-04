/* vim: ft=cpp:ts=2:sts=2:expandtab
 *
 * Struct and classes used in program
 */
#pragma once


// We need the `point` type because the Arduino compiler doesn't accept the
// `Pt` class in PROGMEM: "a variable with dynamic initialization
// cannot be put in program memory area".
typedef struct {
  float x, y;
} point;

class Pt {
  // A point. Named `Pt` to avoid confusion with Arduino `Point` class.
  // and with `point` struct above

  public:
    // I leave these public: less hassle
    float x, y;

    Pt() : x(0), y(0) {}
    Pt(float x, float y) : x(x), y(y) {}
    Pt(const Pt& p) : x(p.x), y(p.y) {}
    Pt(const point& p) : x(p.x), y(p.y) {}

    Pt operator+=(const Pt&);
    Pt operator+(const Pt&);
    Pt operator-(void);
    Pt operator-(const Pt&);
    Pt operator*(float a);
    bool operator==(const Pt&);

    Pt rotate(float alpha);
};

