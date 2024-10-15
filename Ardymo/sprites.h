// vim ft=cpp
//
// Arrow points
#include "defines.h"

enum {
  UP_ARROW = 0,
  RIGHT_ARROW = 8,
  DOWN_ARROW = 16,
  LEFT_ARROW = 24,

};

// sprites/arrow-up.png  width: 8 height: 8
const PROGMEM uint8_t arrows[] = {
// sprites/arrow-up.png  width: 8 height: 8
0x10,0x1c,0x1f,0x1c,0x10,0x00,0x00,0x00,
// sprites/arrow-right.png  width: 8 height: 8
0x1f,0x0e,0x0e,0x04,0x04,0x00,0x00,0x00,
// sprites/arrow-down.png  width: 8 height: 8
0x01,0x07,0x1f,0x07,0x01,0x00,0x00,0x00,
// sprites/arrow-left.png  width: 8 height: 8
0x04,0x04,0x0e,0x0e,0x1f,0x00,0x00,0x00,
};