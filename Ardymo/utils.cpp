#include "utils.h"
#include "shapes.h"

// Get obstacle at index i from PROGMEM obstacles in shapes.h
void get_obstacle(obstacle_t* obst, uint8_t i) {
  memcpy_P(obst, obstacles+i, sizeof(obstacle_t));
#ifndef ARDUINO
  // Byte alignment on x86_64 different from Arduino .
  memcpy((uint8_t*)&(obst->item) + 0xE, (uint8_t*)&(obst->item) + 0x10, 2);
#endif
}
// vim: ft=cpp

