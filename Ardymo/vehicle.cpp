// vim: ts=2:sts=2:expandtab
//
// Vehicle manouvering
//
#include "vehicle.h"
#include "globals.h"
#include "defines.h"

static Vehicle vehicle;

void InitVehicle(rectangle_t rect) {
  vehicle = Vehicle(rect);
}

void TurnRight(void) {
  /* vehicle.direction += kDirectionStep; */
  // TODO: Recompute rectangle
}

void TurnLeft(void) {
  /* vehicle.direction -= kDirectionStep; */
  // TODO: Recompute rectangle
}

void AccelerateForward(void) {
}

void AccelerateBackward(void) {
}

void Vehicle::rotate(float alpha) {
  heading += alpha;
  // Avoid incremental rotations. Will be off due to float precision
  // Practical to precompute. Will be certainly used each frame
  v = Vec(0, length).rotate(heading);
  front = v.rotate(90) * ratio;
}

void Vehicle::move(void) {
  // Move by one unit of speed
  float vlen = p.length();
  p = p + v / vlen * speed;
}
