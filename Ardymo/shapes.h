/* vim: ft=cpp:ts=2:sts=2:expandtab
 * A line segment to be rotated.
 */
#pragma once
#include "stdint.h"
#include "defines.h"
#include "structs.h"

constexpr rectangle_t kVehicle = {
  {kBoardWidth - 10.0, kBoardHeight - 10.0}, 10.0, 90, 5.0};

// Enter the obstacles here. The format used for the `.item` part of
// `obstacle_t` is {float, float, float, int16_t, uint32_t}. The last entry
// represents a `float` for the `rectangle_t` obstacle. In order to
// find the corresponding uint32_t representation,
// use the [IEEE-754 Floating Point Converter]
// (https://www.h-schmidt.net/FloatConverter/IEEE754.html)

// Training: One obstacle, only borders and target. Learn to navigate
// around an obstacle and read sensors.
const obstacle_t PROGMEM _level0[] = {
  // Target: Must be element zero of the obstacles array
  {CIRCLE, 202.5, 202.5, 5.0},
  // Borders of board
  {BORDER, 0.0, 0.0, kBoardWidth, -90, 0x01},  // North
  {BORDER, 0.0, 0.0, kBoardHeight, 0, 0x01},  // West
  {BORDER, 0.0, kBoardHeight, kBoardWidth, -90, 0x01},  // South
  {BORDER, kBoardWidth, 0.0, kBoardHeight, 0, 0x01},  // East
  {CIRCLE, 1000.0, 500.0, 400.0},
};

// Easy: Easy to navigate around obstacles
const obstacle_t PROGMEM _level1[] = {
  // Target: Must be element zero of the obstacles array
  {CIRCLE, 202.5, 202.5, 5.0},
  // Borders of board
  {BORDER, 0.0, 0.0, kBoardWidth, -90, 0x01},  // North
  {BORDER, 0.0, 0.0, kBoardHeight, 0, 0x01},  // West
  {BORDER, 0.0, kBoardHeight, kBoardWidth, -90, 0x01},  // South
  {BORDER, kBoardWidth, 0.0, kBoardHeight, 0, 0x01},  // East
  {CIRCLE, 256.0, 512.0, 128.0},
  {CIRCLE, 256.0, 512.0 + 256.0, 64.0},
  {CIRCLE, 764.0, 384.0, 128.0},
  {CIRCLE, 764.0, 764.0, 128.0},
  {CIRCLE, 764.0 + 512.0, 512.0, 128.0},
  {CIRCLE, 764.0 + 512.0, 512, 128.0},
  {CIRCLE, 764.0 + 512.0, 2 * 256.0, 128.0},
  {CIRCLE, 3 * 512.0, 3 * 256.0, 128.0},
};

// Medium: More complex ways
const obstacle_t PROGMEM _level2[] = {
  // Target: Must be element zero of the obstacles array
  {CIRCLE, 202.5, 202.5, 5.0},
  // Borders of board
  {BORDER, 0.0, 0.0, kBoardWidth, -90, 0x01},  // North
  {BORDER, 0.0, 0.0, kBoardHeight, 0, 0x01},  // West
  {BORDER, 0.0, kBoardHeight, kBoardWidth, -90, 0x01},  // South
  {BORDER, kBoardWidth, 0.0, kBoardHeight, 0, 0x01},  // East
  // Obstacles
  {CIRCLE, 410.0, 220.0, 150.0}, // 1
  {CIRCLE, 750.0, 220.0, 150.0}, // 2
  {CIRCLE, 1110.0, 220.0, 150.0}, // 3
  {CIRCLE, 580.0, 500.0, 150.0}, // 4
  {CIRCLE, 930.0, 500.0, 150.0}, // 5
  {CIRCLE, 350.0, 790.0, 150.0}, // 6
  {CIRCLE, 750.0, 790.0, 150.0}, // 7
  {RECTANGLE, 350.0, 400.0, 150.0, 0, 0x43af0000}, // 8 (w=350.0)
  {RECTANGLE, 820.0, 5.0, 150.0, 225, 0x43160000}, // 9 (w=150.0)
  {RECTANGLE, 1100.0, 1014.0, 150.0, 135, 0x447a0000}, // 10 (w=1000.0)
  {CIRCLE, 920.0, 970.0, 50.0}, // 11
};

// Hard: Dead ends included
const obstacle_t PROGMEM _level3[] = {
  // Target: Must be element zero of the obstacles array
  {CIRCLE, 202.5, 202.5, 5.0},
  // Borders of board
  {BORDER, 0.0, 0.0, kBoardWidth, -90, 0x01},  // North
  {BORDER, 0.0, 0.0, kBoardHeight, 0, 0x01},  // West
  {BORDER, 0.0, kBoardHeight, kBoardWidth, -90, 0x01},  // South
  {BORDER, kBoardWidth, 0.0, kBoardHeight, 0, 0x01},  // East
  // Obstacles
  {CIRCLE, 410.0, 220.0, 150.0}, // 1
  {RECTANGLE, 594.0, 586.0, 300.0, 30, 0x43160000}, // 2 (w=150.0)
  {LINE, 860, 75, 400, 50, 0x01},  // 3
  {LINE, 590, 350, 300, -30, 0x01},  // 4
  {LINE, 865, 550, 300, 30, 0x01},  // 5
  {LINE, 890, 730, 300, -30, 0x01},  // 6
  {LINE, 890, 730, 300, -30, 0x01},  // 6
  {LINE, 965, 504, 300, 0, 0x01},  // 7
  {LINE, 965, 504, 720, 225, 0x01},  // 8
  {LINE, 997, 557, 140, -4, 0x01},  // 9
  {LINE, 997, 557, 140, 245, 0x01},  // 10
  {RECTANGLE, 1040.0, 830.0, 100.0, -60, 0x42960000}, // 11 (w=75.0)
  {CIRCLE, 1120.0, 630.0, 100.0}, // 12
  {LINE, 1075, 1024, 140, 210, 0x01},  // 13
  {CIRCLE, 1118.0, 792.0, 50.0}, // 14
  {LINE, 1150, 894, 300, 200, 0x01},  // 15
  {LINE, 1155, 508, 120, -50, 0x01},  // 16
  {LINE, 1247, 585, 300, 240, 0x01},  // 17
  {LINE, 1252.6, 612.1, 300, 240, 0x01},  // 18
  {LINE, 1530, 1000, 337, 162, 0x01},  // 19
  {LINE, 1506.8, 435, 347.5, -45, 0x01},  // 20
  {LINE, 1512.4, 462.4, 308, -45, 0x01},  // 21
  {RECTANGLE, 1830.0, 700.0, 300.0, 0, 0x43960000}, // 22 (w=300.0)
  {LINE, 1425.8, 680, 304, -90, 0x01},  // 23
  {LINE, 1752.5, 680, 295.5, -90, 0x01},  // 24
  {LINE, 1850, 700, 300, 0, 0x01},  // 25
  {LINE, 1850, 700, 198, -90, 0x01},  // 26
  {LINE, 1850, 1000, 198, -90, 0x01},  // 27
};

// Ultra: Doable. Needs lots of patience
const obstacle_t PROGMEM _level4[] = {
  // Target: Must be element zero of the obstacles array
  {CIRCLE, 202.5, 202.5, 5.0},
  // Borders of board
  {BORDER, 0.0, 0.0, kBoardWidth, -90, 0x01},  // North
  {BORDER, 0.0, 0.0, kBoardHeight, 0, 0x01},  // West
  {BORDER, 0.0, kBoardHeight, kBoardWidth, -90, 0x01},  // South
  {BORDER, kBoardWidth, 0.0, kBoardHeight, 0, 0x01},  // East
  // Obstacles
  {CIRCLE, 410.0, 220.0, 150.0}, // 1
  {RECTANGLE, 594.0, 586.0, 300.0, 30, 0x43160000}, // 2 (w=150.0)
  {LINE, 860, 75, 400, 50, 0x01},  // 3
  {LINE, 590, 350, 300, -30, 0x01},  // 4
  {LINE, 865, 550, 300, 30, 0x01},  // 5
  {LINE, 890, 730, 300, -30, 0x01},  // 6
  {LINE, 890, 730, 300, -30, 0x01},  // 6
  {LINE, 965, 504, 300, 0, 0x01},  // 7
  {LINE, 965, 504, 720, 225, 0x01},  // 8
  {LINE, 997, 557, 140, -4, 0x01},  // 9
  {LINE, 997, 557, 140, 245, 0x01},  // 10
  {RECTANGLE, 1040.0, 830.0, 100.0, -60, 0x42960000}, // 11 (w=75.0)
  {CIRCLE, 1120.0, 630.0, 100.0}, // 12
  {LINE, 1075, 1024, 140, 210, 0x01},  // 13
  {CIRCLE, 1118.0, 792.0, 50.0}, // 14
  {LINE, 1150, 894, 300, 200, 0x01},  // 15
  {LINE, 1155, 508, 120, -50, 0x01},  // 16
  {LINE, 1247, 585, 300, 240, 0x01},  // 17
  {LINE, 1252.6, 612.1, 300, 240, 0x01},  // 18
  {LINE, 1530, 1000, 337, 162, 0x01},  // 19
  {LINE, 1506.8, 435, 347.5, -45, 0x01},  // 20
  {LINE, 1512.4, 462.4, 308, -45, 0x01},  // 21
  {RECTANGLE, 1830.0, 700.0, 300.0, 0, 0x43960000}, // 22 (w=300.0)
  {LINE, 1425.8, 680, 304, -90, 0x01},  // 23
  {LINE, 1752.5, 680, 295.5, -90, 0x01},  // 24
  {LINE, 1850, 700, 300, 0, 0x01},  // 25
  {LINE, 1850, 700, 198, -90, 0x01},  // 26
  {LINE, 1850, 1000, 198, -90, 0x01},  // 27
};

const uint8_t obst_count[kLevels] = {
  sizeof(_level0) / sizeof(obstacle_t),
  sizeof(_level1) / sizeof(obstacle_t),
  sizeof(_level2) / sizeof(obstacle_t),
  sizeof(_level3) / sizeof(obstacle_t),
  sizeof(_level4) / sizeof(obstacle_t),
};
const obstacle_t* const obstacles[kLevels] PROGMEM =
  {_level0, _level1, _level2, _level3, _level4};

