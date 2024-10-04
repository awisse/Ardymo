/* vim: ft=cpp:ts=2:sts=2:expandtab
 *
 * Struct and classes used in program
 */
#pragma once
#include <stdint.h>
#include "pt.h"

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
  point p; // A point on the line
  point v; // Direction of the line
  bool is_segment; // For segments.
} line_t;

typedef struct {
  point p; // Center of the circle
  float r; // radius
  float _dummy0; // Alignment
  bool _dummy1; // Alignment
} circle_t;

typedef struct {
  geometry type;
  union {
    line_t line;
    circle_t circle;
  } object;
} obstacle;

typedef struct {
  Pt p0; // Origin of rectangle
  Pt v0, v1, v2, v3; // Directional vectors
} rectangle_t;

