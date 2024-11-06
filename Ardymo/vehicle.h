#pragma once
#include "defines.h"
#include "objects.h"
#include "structs.h"

// To give it a name ...
constexpr uint8_t COLLIDED {1};

enum distance_t : int16_t {
  FREE,        // No obstacle in sight
  INTERSECTION,// sensor ray intersects an obstacle
  COLLISION    // The vehicle has an intersection with an obstacle
};

// For timeshared sensor calcultions.
// Which direction to check
enum check_t : int16_t {
  LEFT_RIGHT,       // Checked by Map
  FORWARD_REARWARD, // Checked by Ardymo
  BOTH              // Both directions checked by Ardymo if Map not available
};

void initVehicle(uint8_t level);
void getVehicleRect(rectangle_t* rect);
void setVehicleRect(rectangle_t* rect);
Vec getVehiclePos(void);
void turnRight(void);
void turnLeft(void);
void accelerateForward(void);
void accelerateBackward(void);
void moveVehicle(void);

void setLevel(uint8_t level);
void checkSensors(SensorValues* sensors, check_t which);

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
    rectangle_t as_rectangle(void);

    // Setters (for I2C)
    void set_rectangle(const rectangle_t* r);

  private:
    RectVector rect;
    float speed; // Units (m) per second
    float step; // Move quantity per frame
};
// vim: ft=cpp:ts=2:sts=2:expandtab
