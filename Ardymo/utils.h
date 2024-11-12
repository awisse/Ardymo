#pragma once
#include "structs.h"

// Get obstacle from level at index i from PROGMEM obstacles in shapes.h
void get_obstacle(obstacle_t* obst, uint8_t level, uint8_t i);

// Get BORDER associated with level from shapes.h
void get_border(rectangle_t* rect, uint8_t level);
// vim: ft=cpp
