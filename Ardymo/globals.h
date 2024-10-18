// Global variables for the *Game*.
#pragma once
#include <stdint.h>
#include "structs.h"

// Game States
enum State {
  startup, // Application starts
  running, // Player is playing
  menu,    // Menu is open
  success, // Successfully finished
  over     // Game Over
};

typedef struct {
  uint8_t minutes;
  uint8_t seconds;
  uint8_t mdigits; // How many digits in minutes
  uint8_t sdigits; // How many digits in seconds
} Duration;

extern bool modified; // True if screen needs to be redrawn
// vim:ft=cpp
