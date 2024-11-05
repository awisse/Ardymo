// vim: ft=cpp
// Text displayed on Help page.
#include "stdint.h"
#include "src/defines.h"

const uint8_t PROGMEM helpText[] = {
  // Page 1
  "Map Screen\0"
  "\x11   Pan Left\0"
  "\x10   Pan Right\0"
  "\x1e   Pan Up\0"
  "\x1f   Pan Down\0"
  "A+\x1f Zoom In\0"
  "A+\x1e Zoom Out\0"
  "B   Enter Menu\0"
  "\xff"
  // Page 2
  "Menu\0"
  "\x1e   Up\0"
  "\x1f   Down\0"
  "\x11   Next Value\0"
  "\x10   Previous Value\0"
  "A   Select & Exit\0"
  "B   Exit Menu\0"
};
#pragma once
