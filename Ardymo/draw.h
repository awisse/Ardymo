// Drawing/display functions
#pragma once
#include <stdint.h>

// Draw elapsed time and distance to target in top left corner
void DrawScore(int16_t score, uint16_t elapsed);
// Redraw compass arrow with new direction of car with respect to north
void DrawCompass(int16_t vehicle_direction, int16_t target_direction);
// Draw sensor values
void DrawSensors(void);
// vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab:filetype=cpp
