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
  point v; // Direction of the line
  int8_t seg; // seg=1 for segments, seg=0 for lines.
};

struct circle_t {
  point p; // Center of the circle
  float r; // radius
};

struct rectangle_t {
  point p; // Origin of rectangle
  point v; // Direction of rectangle (width = ∥v∥)
  float mu; // height = mu * width
};

struct obstacle {
  geometry type;
  union {
    rectangle_t rectangle; // First declaration must be the most general member
    circle_t circle;
    line_t line;
  };
};

