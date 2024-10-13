#include "globals.h"
#include "draw.h"
#include "platform.h"
#include "shapes.h"

void eraseRect(uint16_t x, uint16_t y, uint8_t width, uint8_t height) {
  // There are 8 rows of bytes and 128 columns of bytes on the screen. Write
  // zeros directly to the screen buffer. 
  // x is translated to 
  // row: Which row {0,...,7} to start with, rows: {1,...8} how many rows
  uint8_t* screen = Platform::getBuffer();
  uint8_t mask;
  uint16_t i; // Index in screen buffer
  uint16_t row = y >> 3; // Byte row on screen

  // Adapt mask to first row
  mask = 0xFF >> (y % 8);
  Platform::DebugPrint("y, row * 0x10 + x + width, mask\n");
  Platform::DebugPrint(y);
  Platform::DebugPrintln();
  Platform::DebugPrint(uint16_t(row * 0x10 + x + width));
  Platform::DebugPrintln();
  Platform::DebugPrint((uint16_t)(mask));
  Platform::DebugPrintln();
  for (i=(y >> 3) * 0x10 + x; i<(y >> 3) * 0x10 + x + width; i++) {
    /* Platform::DebugPrint(x); */
    /* Platform::DebugPrint(y); */
    /* Platform::DebugPrint((uint16_t)width); */
    /* Platform::DebugPrint(i); */ 
    screen[i] &= mask;
  }
}

// Redraw all
__attribute__ ((noinline)) void DrawScreen(void) {
  Platform::fillScreen(COLOUR_WHITE);
  // Erase a small rectangle in the middle
  /* eraseRect(32, 3, 64, 2); */
}
  
// Draw elapsed time and distance to target in top left corner
void DrawScore(void) {
}
   
// Redraw compass arrow with new coordinates
void DrawCompass(void) {
}
 
// Quick erase part of screen buffer. x must be a multiple of 8                       
void EraseSection(int16_t x, int16_t y, uint8_t w, uint8_t h) {
}

// vim:fdm=syntax
