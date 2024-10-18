#pragma once
#include "objects.h"
#include "structs.h"

constexpr float SpeedStep = 0.1;
constexpr int16_t AngleStep = 2;
constexpr float MaxSpeed = 2.0;

void InitVehicle(void);
void TurnRight(void);
void TurnLeft(void);
void AccelerateForward(void);
void AccelerateBackward(void);
void MoveVehicle(void);

void CheckSensors(SensorValues* sensors);

class Vehicle {

  public:
    // Constructors
    Vehicle() : Vehicle(Vec(0,0), 1.0, 0, 0.5) {}

    Vehicle(Vec p, float l, int16_t h, float mu) :
      p(p), length(l), heading(h), ratio(mu), width(l * mu),
      speed(0.0f) {turn(0);}

    Vehicle(float x, float y, float l, int16_t h, float mu) :
      Vehicle(Vec(x, y), l, h, mu) {} // Constructor delegation

    Vehicle(rectangle_t rect) :
      Vehicle(Vec(rect.p), rect.w, rect.rho, rect.mu) {}

    // Methods
    void turn(float alpha); // Rotate around center of rear
    void move(void); // Move in direction by present speed.
    bool collided(obstacle obst); // Collision with obstacles
    rectangle_t as_rectangle(void);
    void accelerate_forward(void);
    void accelerate_backwards(void);
    
    // Getters
    float get_speed(void) {return speed;}

    // Public variables (no messing with getters, setters)
    Vec p; // Origin of rectangle
    Vec v; // Direction of rectangle
    Vec front; // Vector defining the front of the vehicle
    float length, width, ratio; // Length, width / length of vehicle
    int16_t heading;

  private:
    float speed; // Forward (positive) or backward (negative)
    float forward, left, right, rearward; // Distance to obstacles
};
// vim: ft=cpp:ts=2:sts=2:expandtab
