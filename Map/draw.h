// Drawing/display functions
#pragma once
#include <stdint.h>
#include "src/structs.h"
#include "src/objects.h"

// Redraw Viewport. The vehicle position might have changed
void Draw(rectangle_t* vehicle);
// Draw position in bottom left corner.
void DrawPosition(point* vehicle_position);

// vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab:filetype=cpp
