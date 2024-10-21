// vim: ts=2:sts=2:expandtab
//
// Vehicle manouvering
//
#ifdef ARDUINO
#include "math.h"
#else
#include <limits>
#define INFINITY std::numeric_limits<float>::infinity()
#endif
#include "vehicle.h"
#include "intersection.h"
#include "globals.h"
#include "defines.h"
#include "shapes.h"

static Vehicle vehicle;

// Directions to check for distance
enum direction_t {
  FORWARD = 0,
  BACKWARD = 1
};

// Helper functions

void GetDistances(LineVector sensor, float* distances) {
  // Compute distance to all obstacles.
  // Return the closest
  obstacle_t obst;
  intersection_t x; // Point of intersection
  float d;
  size_t obstacle_count = sizeof(obstacles) / sizeof(obstacle_t);
  int16_t i, j, intersection_count;

  // Set Distances to infinity
  distances[0] = distances[1] = INFINITY;
  for (i=0; i<obstacle_count; i++) {
    memcpy_P(&obst, obstacles + i, sizeof(obstacle_t));
#ifndef ARDUINO
    // Byte alignment on Arduino different from 64-bit intel.
    memcpy((uint8_t*)&obst.item + 0xE, (uint8_t*)&obst.item + 0x10, 2);
#endif
    intersection_count = intersects(sensor, obst);
    for (j=0; j<intersection_count; j++) {
      x = intersect_point(j);
      d = distance(sensor.p, j);
      if ((x.tau > 0) && (d < distances[FORWARD])) {
        distances[FORWARD] = d;
      } else if (d < distances[BACKWARD]) {
        distances[BACKWARD] = d;
      }
    }
  }
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
  float d; // Temporary distance variable
  float distances[2];

  // Left
  sensor = LineVector(vehicle.p, vehicle.v, vehicle.length, 0);
  // FORWARD
  GetDistances(sensor, distances);
  sensors->distances.front = distances[FORWARD] - vehicle.length;
  // REARWARD
  sensors->distances.rear = distances[BACKWARD];

  // Right
  sensor = LineVector(vehicle.p + vehicle.front, vehicle.v, 
      vehicle.length, 0);
  GetDistances(sensor, distances);
  // FORWARD
  if (distances[FORWARD] < (sensors->distances.right - vehicle.width)) {
    sensors->distances.right = distances[FORWARD] - vehicle.width;
  }
  // REARWARD
  if (distances[BACKWARD] < sensors->distances.left) {
    sensors->distances.left = distances[BACKWARD];
  }
  
  // Front
  sensor = LineVector(vehicle.p + vehicle.v, vehicle.front, 
      vehicle.width, 0);
  GetDistances(sensor, distances);
  // Right
  sensors->distances.right = distances[FORWARD] - vehicle.width;
  // Left
  sensors->distances.left = distances[BACKWARD];

  // Rear
  sensor = LineVector(vehicle.p, vehicle.front, vehicle.width, 0);
  GetDistances(sensor, distances);
  // Right
  if (distances[FORWARD] < (sensors->distances.right - vehicle.width)) {
    sensors->distances.right = distances[FORWARD] - vehicle.width;
  }
  // Left
  if (distances[BACKWARD] < sensors->distances.left) {
    sensors->distances.left = distances[BACKWARD];
  }

  sensors->heading = vehicle.v.normalized().as_point();
  // Position: Center of front
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
  p += v * step;
}

void Vehicle::accelerate_forward(void) {
  speed = speed >= MaxSpeed ? MaxSpeed : speed + SpeedStep;
  step = speed / 1000.0 * kFrameDuration / length;
}

void Vehicle::accelerate_backwards(void) {
  // Max backward speed = half max speed
  speed = speed <= -MaxSpeed / 2.0 ? - MaxSpeed / 2.0 : speed - SpeedStep;
  step = speed / 1000.0 * kFrameDuration / length;
}
