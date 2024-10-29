#pragma once
#include "structs.h"

// Get obstacle at index i from PROGMEM obstacles in shapes.h
void get_obstacle(obstacle_t* obst, uint8_t i);

// vim: ft=cpp
