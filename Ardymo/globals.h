// Global variables for the *Game*.
#pragma once
#include <stdint.h>
#include "structs.h"
/* #include "defines.h" */

// Game States
enum State {
  startup, // Application starts
  running, // Player is playing
  menu,    // Menu is open
  success, // Successfully finished
  over     // Game Over
};

// Sensor parameters. If distance=0: Collision.
extern struct {
  float front;  // Distance to closest obstacle ahead.
  float left;   // Distance to closest obstacle to left
  float rear;   // Distance to closest obstacle behind
  float right;  // Distance to closest obstacle to right
  float heading;// Compass heading of the vehicle
  float target; // Distance to target
  float target_direction; // Target direction with respect to vehicle axis (angle)
} sensors;

typedef struct {
  uint8_t minutes;
  uint8_t seconds;
  uint8_t mdigits; // How many digits in minutes
  uint8_t sdigits; // How many digits in seconds
} Duration;

extern bool modified; // True if screen needs to be redrawn
// vim:ft=cpp
