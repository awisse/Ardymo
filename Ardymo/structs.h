/* vim: ft=cpp:ts=2:sts=2:expandtab
 *
 * Struct and classes used in program
 */
#pragma once
#include <stdint.h>
#include "objects.h"

typedef enum {
  LINE,
  CIRCLE,
  RECTANGLE,
} geometry;

typedef enum {
  VEHICLE,
  OBSTACLE,
  TARGET
} object_t;

// We must define our obstacles as a union of structs instead of a class
// because the obstacles will be saved in PROGMEM.
typedef struct {
  point p; // A point on the line
  point v; // Direction of the line
  int8_t seg; // seg=1 for segments, seg=0 for lines.
} line_t;

typedef struct {
  point p; // Center of the circle
  float r; // radius
} circle_t;

typedef struct {
  point p; // Origin of rectangle
  point v; // Direction of rectangle (width = ∥v∥)
  float mu; // height = mu * width
} rectangle_t;

typedef struct {
  geometry type;
  union {
    rectangle_t rectangle; // First declaration must be the most general member
    circle_t circle;
    line_t line;
  };
} obstacle;

