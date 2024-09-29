/* vim: ft=cpp:ts=2:sts=2:expandtab
 *
 * Struct and classes used in program
 */
#pragma once
#include <stdint.h>


// We need the `point` type because the Arduino compiler doesn't accept the
// `Pt` class in PROGMEM: "a variable with dynamic initialization
// cannot be put in program memory area".
typedef struct {
  int16_t x, y;
} point;

typedef enum {
  LINE,
  RECTANGLE,
  CIRCLE,
} geometry;

typedef enum {
  VEHICLE,
  OBSTACLE,
  TARGET
} object_t;

// We must define our obstacles as a union of structs instead of a class
// because the obstacles will be saved in PROGMEM.
typedef struct {
  float a, b; // coefficients of ax+b
  float X; // Parameter for vertical line
  float z0, z1; // For segments. z0=z1=0 for lines
} line_t;

typedef struct {
  float x0, y0; // Center of left side
  float w, h; // Width (x), Height(0)
  float alpha; // Angle with respect to x-axis
} rectangle_t;

typedef struct {
  float x0, y0; // Center of the circle
  float r; // radius
  float _dummy0; // Alignment
  float _dummy1; // Alignment
} circle_t;

typedef struct {
  geometry type;
  union {
    line_t line;
    rectangle_t segment;
    circle_t circle;
  } object;
} obstacle;


class Pt {
  // A point. Named `Pt` to avoid confusion with Arduino `Point` class.
  // and with `point` struct above

  public:
    // I leave these public: less hassle
    int16_t x, y;

    Pt() : x(0), y(0) {}
    Pt(int16_t x, int16_t y) : x(x), y(y) {}
    Pt(const Pt& p) : x(p.x), y(p.y) {}
    Pt(const point& p) : x(p.x), y(p.y) {}

    Pt operator+=(const Pt&);
    Pt operator+(const Pt&);
    Pt operator-(void);
    Pt operator-(const Pt&);
    Pt operator*(float a);
    bool operator==(const Pt&);
};

