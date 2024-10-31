#pragma once
#include "src/defines.h"

void InitGame();
void StepGame(); // Return true if screen must be updated
void Restart();
void Terminate();
void ToggleCoordinates();
void ToggleFollow();
// vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab:filetype=cpp
