/* vim: ft=cpp:ts=2:sts=2:expandtab
 *
 * Structs and classes used for the intersection algorithm
 */
#pragma once
#include <stdint.h>
#include "objects.h"

enum geometry {
  LINE,
  CIRCLE,
  RECTANGLE,
};

enum side_t {
  NONE = 0,
  LEFT,
  FRONT,
  RIGHT,
  REAR,
  NUM // Number of elements in side_t
};

// We must define our obstacles as a union of structs instead of a class
// because the obstacles will be saved in PROGMEM.
// First a structure to use for the PROGMEM area
struct progmem_t {
  point p;
  float f;
  int16_t i;
  uint32_t bytes; // Holds a byte, a float an int16_t and a int32_t
};

struct line_t {
  point p; // A point on the line
  float l; // Length of line (if segment)
  int16_t rho; // Rotation of line with respect to the south (0,1).
  uint8_t seg; // seg=1 for segments, seg=0 for lines.
};

struct circle_t {
  point p; // Center of the circle
  float r; // radius
};

struct rectangle_t {
  point p; // Origin of rectangle
  float l; // Length of rectangle (in the direction determined by rho)
  int16_t rho; // Direction of the rectangle with respect to the south (0,1).
  float w; // Width of rectangle (perpendicular to the direction)
};

struct obstacle_t {
  geometry type;
  union {
    // Start with a generic struct for initialization of PROGMEM area
    progmem_t _progmem_; // Generic. Unused.
    rectangle_t rectangle; // First declaration must be the most general member
    circle_t circle;
    line_t line;
  } item;
};

struct LineVector { // for sensor rays

  LineVector() : LineVector(Vec(), 0.0, 0, 0) {}
  LineVector(Vec p, Vec v, float l, uint8_t s) : p(p), v(v), l(l), seg(s) {}
  LineVector(Vec p, Vec v, uint8_t s) :
    p(p), v(v), l(v.length()), seg(s) {}
  LineVector(Vec p, float l, int16_t rho, uint8_t s) :
    p(p), v(Vec(0, l).rotate(rho)), l(l), seg(s) {}
  LineVector(line_t line) :
    LineVector(Vec(line.p), line.l, line.rho, line.seg) {}

  Vec p; // Origin on the sensor ray
  Vec v; // Directional vector, computed from l and rho
  float l; // Length of direction vector
  uint8_t seg; // In case we want to use this for a segment
};

struct RectVector { // A rectangle in vector format
  Vec p; // Origin of rectangle, like in rectangle_t
  Vec v; // "Left" side of the rectangle and direction
  Vec front; // Side perpendicular to v,
  float l; // Length of the rectangle (length of v)
  int16_t rho; // Same as rectangle_t
  float w; // Width of the rectangle (length of front)
  RectVector() = default;
  RectVector(rectangle_t rect) :
    p(Vec(rect.p)), v(Vec(0, rect.l).rotate(rect.rho)),
    front(Vec(0, rect.w).rotate(90 +rect.rho)), l(rect.l),
    rho(rect.rho), w(rect.w) {}
};


// Distances to obstacles and target
struct Distances {
  float left, front, right, rear;
};

struct SensorValues {
  // Distances to obstacles
  Distances distances;
  // Heading as coordinates. That's how it will be displayed on the compass
  Vec heading;
  int16_t alpha; // Heading in degrees (for display)
  // Vehicle position: center of front of vehicle
  Vec position;
  // Vehicle speed
  float speed;
  // Collision
  side_t collision;
};

// vim:ft=cpp
