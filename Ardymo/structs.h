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

enum object_t {
  VEHICLE,
  OBSTACLE,
  TARGET
} ;

enum side_t {
  NONE = 0,
  LEFT,
  FRONT,
  RIGHT,
  REAR,
  NUM // Number of sides + 1 (for NONE)
};

// We must define our obstacles as a union of structs instead of a class
// because the obstacles will be saved in PROGMEM.
struct line_t {
  point p; // A point on the line
  float l; // Length of line (if segment)
  float rho; // Rotation of line with respect to the south (0,1).
  int8_t seg; // seg=1 for segments, seg=0 for lines.
};

struct circle_t {
  point p; // Center of the circle
  float r; // radius
};

struct rectangle_t {
  point p; // Origin of rectangle
  float w; // Width of rectangle (length of vehicle)
  float rho; // Rotation of rectangle with respect to the north (0,1).
  float mu; // height = mu * w (height = width of vehicle)
};

struct obstacle {
  geometry type;
  union {
    rectangle_t rectangle; // First declaration must be the most general member
    circle_t circle;
    line_t line;
  };
};

struct LineVector { // for sensor rays

  LineVector() : LineVector(Vec(), 0.0, 0) {}
  LineVector(Vec p, Vec v) : p(p), v(v), l(v.length()), seg(true) {}
  LineVector(Vec p, float l, float rho) : p(p), v(Vec(0, l).rotate(rho)),
                                                 seg(true) {}
  LineVector(line_t line) :
    LineVector(Vec(line.p), line.l, line.rho) {seg = line.seg;}

  Vec p; // Origin on the sensor ray
  Vec v; // Directional vector, computed from l and rho
  float l; // Length of direction vector
  bool seg; // In case we want to use this for a segment
};

