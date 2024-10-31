// Drawing/display functions
#pragma once
#include <stdint.h>
#include "structs.h"
#include "objects.h"

// Draw parts of screen that don't change during updates
void DrawBackground(void);
// Draw speed and distance to target in top left corner
void DrawStatus(float speed, float distance);
// Redraw compass arrow with new direction of car with respect to north
// and direction of target with respect to vehicle axis.
void DrawCompass(Vec vehicle_direction, int16_t heading, 
    Vec target_direction);
// Draw vehicle position in bottom left corner.
void DrawPosition(Vec vehicle_position);
// Draw four distances as measured by sensors
void DrawDistances(Distances*);
// Basic success screen
void DrawSuccess(uint16_t elapsed);
// Basic Game Over screen
void DrawGameOver(uint16_t elapsed);
void DrawCrash(Vec* position);
// Draw a message in the center of the screen
void DrawMessage(const char* msg);

// vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab:filetype=cpp
