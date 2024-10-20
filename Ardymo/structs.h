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
  float w; // Width of rectangle (length of vehicle)
  int16_t rho; // Rotation of rectangle with respect to the north (0,1).
  float mu; // height = mu * w (height = width of vehicle)
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
