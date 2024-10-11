// Drawing/display functions
#pragma once
#include <stdint.h>

// Redraw all
void DrawScreen(void);  
// Draw elapsed time and distance to target in top left corner
void DrawScore(void);   
// Redraw compass arrow with new coordinates
void DrawCompass(void); 
// Quick erase part of screen buffer. x must be a multiple of 8                       
void EraseSection(int16_t x, int16_t y, uint8_t w, uint8_t h); 
// vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab:filetype=cpp
