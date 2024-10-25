// Drawing/display functions
#pragma once
#include <stdint.h>
#include "structs.h"
#include "objects.h"

void DrawBackground(void);
// Redraw Viewport. The vehicle position might have changed
void Draw(rectangle_t* vehicle);
// Draw speed and distance to target in top left corner
void DrawStatus(float speed, float distance);
// Draw position in bottom left corner.
void DrawPosition(point* vehicle_position);

// vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab:filetype=cpp
