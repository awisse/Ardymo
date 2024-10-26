#pragma once

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

void HandleInput();

// vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab:filetype=cpp
