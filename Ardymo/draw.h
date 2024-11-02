// Drawing/display functions
#pragma once
#include <stdint.h>
#include "structs.h"
#include "objects.h"

// Draw parts of screen that don't change during updates
void drawBackground(void);

// Draw speed and distance to target in top left corner
void drawStatus(float speed, float distance);

// Redraw compass arrow with new direction of car with respect to north
// and direction of target with respect to vehicle axis.
void drawCompass(Vec vehicle_direction, int16_t heading, Vec target_direction);

// Draw vehicle position in bottom left corner.
void drawPosition(Vec vehicle_position);

// Draw four distances as measured by sensors
void drawDistances(Distances*);

// Basic success screen
void drawSuccess(uint16_t elapsed);

// Basic Game Over screen
void drawGameOver(uint16_t elapsed);

void drawCrash(Vec* position);

// Draw a message in the center of the screen
void drawMessage(const char* msg);

// Draw the menu
void drawMenu(uint8_t selected, const char* items[]);
// vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab:filetype=cpp
