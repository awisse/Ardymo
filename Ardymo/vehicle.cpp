// vim: ts=2:sts=2:expandtab
//
// Vehicle manouvering
//
#include "vehicle.h"
#include "intersection.h"
#include "globals.h"
#include "defines.h"
#include "shapes.h"

static Vehicle vehicle;

// Helper functions

float GetDistance(LineVector sensor) {
  // Compute distance to all obstacles.
  // Return the closest
  obstacle_t obst;
  float d, closest = 1.0 / 0.0;
  size_t obstacle_count = sizeof(obstacles) / sizeof(obstacle_t);
  int16_t i, j, intersection_count;

  for (i=0; i<obstacle_count; i++) {
    memcpy_P(&obst, obstacles + i, sizeof(obstacle_t));
#ifndef ARDUINO
    // Byte alignment on Arduino different from 64-bit intel.
    memcpy((uint8_t*)&obst.item + 0xE, (uint8_t*)&obst.item + 0x10, 2);
#endif
    intersection_count = intersects(sensor, obst);
    for (j=0; j<intersection_count; j++) {
      d = distance(sensor.p, j);
      if (d < closest)
        closest = d;
    }
  }
  return closest;
}

void InitVehicle() {
  vehicle = Vehicle(kVehicle);
}

void TurnRight(void) {
  vehicle.turn(AngleStep);
}

void TurnLeft(void) {
  vehicle.turn(-AngleStep);
}

void AccelerateForward(void) {
  vehicle.accelerate_forward();
}

void AccelerateBackward(void) {
  vehicle.accelerate_backwards();
}

void MoveVehicle(void) {
  vehicle.move();
}

void CheckSensors(SensorValues* sensors) {
  // There are in principle eight sensor rays 
  // emanating from the vehicle, two from each corner parallel
  // to the sides of the vehicle (see Figure 7 in ardymo.pdf).
  LineVector sensor;
  float distance;

  // Front Left-Forward
  sensor = LineVector(vehicle.p + vehicle.v, vehicle.v, vehicle.length, 0);
  distance = GetDistance(sensor);
  sensors->distances.front = distance;
  // Front Left-Left
  sensor = LineVector(vehicle.p + vehicle.v, -vehicle.front, 
      vehicle.width, 0);
  sensors->distances.left = 0.0; 
  // Front Right-Forward
  sensor = LineVector(vehicle.p + vehicle.v + vehicle.front, vehicle.v, 
      vehicle.length, 0);
  // Front Right-Right
  sensor = LineVector(vehicle.p + vehicle.v + vehicle.front, vehicle.front, 
      vehicle.width, 0);
  sensors->distances.right = 0.0;
  // Rear Left-Rearward
  sensor = LineVector(vehicle.p, -vehicle.v, vehicle.length, 0);
  sensors->distances.rear = 0.0;
  // Rear Left-Left
  sensor = LineVector(vehicle.p , -vehicle.front, vehicle.width, 0);
  // Rear Right-Rearward
  sensor = LineVector(vehicle.p + vehicle.front, -vehicle.v, vehicle.length, 0);
  // Rear Right-Right
  sensor = LineVector(vehicle.p + vehicle.front, vehicle.front, vehicle.width, 0);

  sensors->heading = vehicle.v.normalized().as_point();
  sensors->position = vehicle.p + vehicle.v + vehicle.front / 2.0;
  sensors->alpha = vehicle.heading;
  sensors->speed = vehicle.get_speed();
}

void Vehicle::turn(float alpha) {
  // Avoid incremental rotations. Will be off due to float precision
  heading += alpha;
  if (heading > 180) {
    heading -= 360;
  } else if (heading < -180) {
    heading += 360;
  }
  v = Vec(0, length).rotate(heading);
  // Turning around the center of the rear of the vehicle, which is
  Vec rear_center = p + front / 2.0;
  // Now turn front and use it to recompute p
  front = v.rotate(90) * ratio;
  p = rear_center - front / 2.0;
}

void Vehicle::move(void) {
  // Move by one unit of speed
  p += v * speed;
}

void Vehicle::accelerate_forward(void) {
  speed = speed >= MaxSpeed ? MaxSpeed : speed + SpeedStep;
}

void Vehicle::accelerate_backwards(void) {
  // Max backward speed = half max speed
  speed = speed <= -MaxSpeed / 2.0 ? - MaxSpeed / 2.0 : speed - SpeedStep;
}

