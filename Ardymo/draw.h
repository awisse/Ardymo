// Drawing/display functions
#pragma once
#include <stdint.h>
#include "structs.h"
#include "objects.h"

void DrawBackground(void);
// Draw speed and distance to target in top left corner
void DrawScore(float speed, float distance);
// Redraw compass arrow with new direction of car with respect to north
void DrawCompass(Vec vehicle_direction, int16_t alpha, Vec target_direction);
// Draw sensor values
void DrawDistances(Distances*);
// vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab:filetype=cpp
