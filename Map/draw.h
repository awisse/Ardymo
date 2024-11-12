// Drawing/display functions
#pragma once
#include <stdint.h>
#include "src/structs.h"
#include "src/objects.h"

// Redraw Viewport. The vehicle position might have changed
void draw(vehicle_t* vehicle);
// Draw position in bottom left corner.
void drawPosition(point* vehicle_position);
// Draw startup screen
void drawStartup();
// Draw the menu
void drawMenu(uint8_t selected, uint8_t numItems, const char* items[]);
// Draw Help
void drawHelp(uint8_t page);
// vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab:filetype=cpp
