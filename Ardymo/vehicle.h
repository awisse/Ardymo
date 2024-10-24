#pragma once
#include "defines.h"
#include "objects.h"
#include "structs.h"

class Vehicle;

void InitVehicle(void);
void TurnRight(void);
void TurnLeft(void);
void AccelerateForward(void);
void AccelerateBackward(void);
void MoveVehicle(void);
// Collision of rectangle with an object
side_t collides(Vehicle, obstacle_t);

void CheckSensors(SensorValues* sensors);

class Vehicle {

  public:
    // Constructors
    Vehicle() = default;

    Vehicle(Vec p, float l, int16_t rho, float w) :
      p(p), v(Vec(0, l).rotate(rho)), length(l), heading(rho),
      width(w), front(Vec(0, w).rotate(rho + 90)), speed(0.0f) {}

    Vehicle(float x, float y, float l, int16_t h, float w) :
      Vehicle(Vec(x, y), l, h, w) {} // Constructor delegation

    Vehicle(rectangle_t rect) :
      Vehicle(Vec(rect.p), rect.w, rect.rho, rect.w) {}

    // Methods
    void turn(float alpha); // Rotate around center of rear
    void move(void); // Move in direction by present speed.
    bool collided(obstacle_t obst); // Collision with obstacles
    rectangle_t as_rectangle(void);
    void accelerate_forward(void);
    void accelerate_backwards(void);

    // Getters
    float get_speed(void) {return speed;}

    // Public variables (no messing with getters, setters)
    Vec p; // Origin of rectangle
    Vec v; // Direction of rectangle
    Vec front; // Vector defining the front of the vehicle
    float length, width; // Length, width of vehicle
    int16_t heading;

  private:
    float speed; // Units (m) per second
    float step; // Move quantity per frame
};
// vim: ft=cpp:ts=2:sts=2:expandtab
