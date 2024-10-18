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

float GetDistance(LineVector sensor) {
  // Compute distance to all obstacles.
  // Return the closest.
  float d, closest = 1.0 / 0.0;
  size_t obstacle_count = sizeof(obstacles) / sizeof(obstacle);
  int16_t i, j, intersection_count;

  for (i=0; i<obstacle_count; i++) {
    intersection_count = intersects(sensor, obstacles[i]);
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

  // Front Left-Forward
  sensor = LineVector(vehicle.p + vehicle.v, vehicle.v, vehicle.length);
  // Front Left-Left
  sensor = LineVector(vehicle.p + vehicle.v, -vehicle.front, 
      vehicle.width);
  // Front Right-Forward
  sensor = LineVector(vehicle.p + vehicle.v + vehicle.front, vehicle.v, 
      vehicle.length);
  // Front Right-Right
  sensor = LineVector(vehicle.p + vehicle.v + vehicle.front, vehicle.front, 
      vehicle.width);
  // Rear Left-Rearward
  sensor = LineVector(vehicle.p, -vehicle.v, vehicle.length);
  // Rear Left-Left
  sensor = LineVector(vehicle.p , -vehicle.front, vehicle.width);
  // Rear Right-Rearward
  sensor = LineVector(vehicle.p + vehicle.front, -vehicle.v, vehicle.length);
  // Rear Right-Right
  sensor = LineVector(vehicle.p + vehicle.front, vehicle.front, vehicle.width);

  sensors->heading = vehicle.v.normalized().as_point();
  sensors->position = vehicle.p + vehicle.v + vehicle.front / 2.0;
  sensors->alpha = vehicle.heading;
  sensors->speed = vehicle.get_speed();
}

void Vehicle::turn(float alpha) {
  // Avoid incremental rotations. Will be off due to float precision
  heading = (heading + (int16_t)alpha) % 360;
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
