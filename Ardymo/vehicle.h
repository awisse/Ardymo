#pragma once
#include "objects.h"
#include "structs.h"

class Vehicle {

  public:
    // Constructors
    Vehicle() : Vehicle(Vec(0,0), 1.0, 0, 0.5) {}

    Vehicle(Vec p, float l, int16_t h, float mu) :
      p(p), length(l), heading(h), ratio(mu), speed(0.0f) {rotate(0);}

    Vehicle(float x, float y, float l, int16_t h, float mu) :
      Vehicle(Vec(x, y), l, h, mu) {} // Constructor delegation

    Vehicle(rectangle_t rect) :
      Vehicle(Vec(rect.p), rect.w, rect.rho, rect.mu) {}

    // Methods
    void rotate(float alpha); // Rotate around center of rear
    void move(void); // Move in direction by present speed.
    float scan(void); // TBD: Scanning functions.
    bool collided(obstacle obst); // Collision with obstacles
    rectangle_t as_rectangle(void);

    // Public variables (no messing with getters, setters)
    Vec p; // Origin of rectangle
    Vec v; // Direction of rectangle
    Vec front; // Vector defining the front of the vehicle
    float length, ratio; // Length, width / length of vehicle
    int16_t heading;

  private:
    float speed; // Forward (positive) or backward (negative)
    float forward, left, right, rearward; // Distance to obstacles
};

void InitVehicle(rectangle_t rect);
void TurnRight(void);
void TurnLeft(void);
void AccelerateForward(void);
void AccelerateBackward(void);

// vim:ft=cpp
