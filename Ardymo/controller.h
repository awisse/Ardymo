#pragma once
#include "structs.h"

/* Key presses */
typedef enum {
  Short_A,
  Long_A,
  Short_B,
  Long_B,
  /* VeryLong_B, */
  Left,
  Right,
  Up,
  Down
} Event;

// Buttons
#define INPUT_LEFT 32u
#define INPUT_RIGHT 64u
#define INPUT_UP 128u
#define INPUT_DOWN 16u
#define INPUT_A 8u
#define INPUT_B 4u

// Time
#define LONG_PRESS 2000

void handleInput(State);

// vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab:filetype=arduino
