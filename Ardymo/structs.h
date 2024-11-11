/* vim: ft=cpp:ts=2:sts=2:expandtab
 *
 * Structs and classes used for the intersection algorithm
 */
#ifndef STRUCTS_H
#define STRUCTS_H
#include <stdint.h>
#include "objects.h"

// Game States
enum State : uint8_t {
  startup, // Application starts
  running, // Player is playing
  gamemenu,    // Menu is open
  startupmenu, // Menu is open
  help,    // Show help screen
  success, // Successfully finished
  crash,   // Crashed into obstacle
  over,    // Game Over
  waiting  // Waiting for Input
};

// Obstacle types
enum geometry : uint16_t { // uint8_t is enough. uint16_t aligns better.
  ANY,
  BORDER,
  TARGET,
  VEHICLE,
  LINE,
  CIRCLE,
  RECTANGLE,
};

// Recangle sides. Also used to show on which side a collision occurred.
enum side_t : uint16_t {
  NONE = 0,
  LEFT,
  FRONT,
  RIGHT,
  REAR,
  NUM, // Number of usable elements (5)
  INVALID = 0xFFFF // For I2C data check
};

// We must define our obstacles as a union of structs instead of a class
// because the obstacles will be saved in PROGMEM.
// First a structure to use for the PROGMEM area. This is used by the
// compiler as the default.
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
  // Direction of the rectangle with respect to the south (0,1).
  // This convention is chosen in order for the vehicle pointing south
  // corresponding to an angle of zero for computations.
  int16_t rho; 
  float w; // Width of rectangle (perpendicular to the direction)
};

struct obstacle_t {
  geometry type;
  union {
    // Start with a generic struct for initialization of PROGMEM area
    progmem_t _progmem_; // Generic. Unused.
    rectangle_t rectangle; 
    circle_t circle;
    line_t line;
  } item;
};

struct LineVector { // For sensor rays. Turns out rather big in the code :-(

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

struct RectVector { // A rectangle in vector format. For storage of
                    // precomputed values.
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
  rectangle_t as_rectangle(void) {return {p.as_point(), l, rho, w};}
};

// Vehicle data to transmit from Ardymo to Map
struct vehicle_t {
  rectangle_t rect;
  uint8_t level;
};

// Distances to obstacles and target
struct Distances {
  float left, right, front, rear;
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
  // Target
  // Distance
  float tgt_distance;
  Vec tgt_heading; // Direction of target with respect to vehicle axis

  // Collision with target
  bool on_target;
};

struct SharedData { // Data computed by Map and shared to Ardymo
  float dist_left, dist_right; // Distances to obstacles left and right
  side_t collision; // Collision detected left or right
  bool on_target; // Vehicle crashed into target
};

#endif // STRUCTS_H
// vim:ft=cpp
