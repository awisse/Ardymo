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

    Vehicle(rectangle_t rect) : rect(RectVector(rect)), speed(0.0),
      step(0.0) {}

    // Methods
    void turn(float alpha); // Rotate around center of rear
    void move(void); // Move in direction by present speed.
    bool collided(obstacle_t obst); // Collision with obstacles
    rectangle_t as_rectangle(void);
    void accelerate_forward(void);
    void accelerate_backwards(void);

    // Getters
    float get_speed(void) const {return speed;}
    Vec p(void) const {return rect.p;}
    Vec v(void) const {return rect.v;}
    Vec front(void) const {return rect.front;}
    float length(void) const {return rect.l;}
    float width(void) const {return rect.w;}
    int16_t rho(void) const {return rect.rho;}

  private:
    RectVector rect;
    float speed; // Units (m) per second
    float step; // Move quantity per frame
};
// vim: ft=cpp:ts=2:sts=2:expandtab
