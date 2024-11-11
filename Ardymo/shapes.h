/* vim: ft=cpp:ts=2:sts=2:expandtab
 * A line segment to be rotated.
 */
#pragma once
#include "stdint.h"
#include "defines.h"
#include "structs.h"

enum vehicle_name : uint8_t {
  VEHICLE_BR2048 = 0, // Bottom right
  VEHICLE_BR1024 = 1, // Bottom right
  VEHICLE_TL = 2  // Top Left
};

constexpr uint8_t t_ix {0}; // Index of target in obstacle Array
constexpr uint8_t o_ix {0}; // Index of first obstacle in obstacle Array

// Three vehicle locations: 
constexpr rectangle_t vehicleLocation[] = {
  {{2038.0, 1014.0}, 10.0, 90, 5.0},
  {{1014.0, 1014.0}, 10.0, 90, 5.0},
  {{10.0, 10.0}, 10.0, -90, 5.0}
};

constexpr vehicle_name vehicleChoice[kLevels] = {
  VEHICLE_BR1024, VEHICLE_BR2048, VEHICLE_BR2048, VEHICLE_BR2048, VEHICLE_TL
};


// Enter the obstacles here. The format used for the `.item` part of
// `obstacle_t` is {float, float, float, int16_t, uint32_t}. The last entry
// represents a `float` for the `rectangle_t` obstacle. In order to
// find the corresponding uint32_t representation,
// use the [IEEE-754 Floating Point Converter]
// (https://www.h-schmidt.net/FloatConverter/IEEE754.html)

// 1. The first (0th) element of the array must be the target.
// 2. The second element must be the rectangle of the four borders

// Training: One obstacle, only borders and target. Learn to navigate
// around an obstacle and read sensors.
const obstacle_t PROGMEM _level0[] = {
  // Target: Must be element  zero of the obstacles array
  {TARGET, 202.5, 202.5, 5.0},
  // Borders of board
  {BORDER, 0.0, 0.0, 1024.0, -90, 0x44800000},  // kBoardWidth x 1024
  // Sole obstacle for Training
  {CIRCLE, 500.0, 500.0, 400.0},
};

// Easy: Easy to navigate around obstacles
const obstacle_t PROGMEM _level1[] = {
  // Target: Must be element  zero of the obstacles array
  {TARGET, 202.5, 202.5, 5.0},
  // Borders of board
  {BORDER, 0.0, 0.0, kBoardWidth, -90, 0x44800000},  // kBoardWidth x 1024
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
  // Target: Must be element  zero of the obstacles array
  {TARGET, 202.5, 202.5, 5.0},
  // Borders of board
  {BORDER, 0.0, 0.0, kBoardWidth, -90, 0x44800000},  // kBoardWidth x 1024
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
  // Target: Must be element  zero of the obstacles array
  {TARGET, 202.5, 202.5, 5.0},
  // Borders of board
  {BORDER, 0.0, 0.0, kBoardWidth, -90, 0x44800000},  // kBoardWidth x 1024
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
  // 39 obstacles from "../doc/odg/map-ultra.svg"
  // Representing 702 bytes
  /* *** Don't forget to put the target first manually *** */
  // Target: Must be element zero of the obstacles array
  {TARGET, 512.0, 512.0, 5.0},
  {BORDER, 0.0, 0.0, 1024.0, -90, 0x44800000},  // 1024 x 1024
  // Obstacles
  {RECTANGLE, 570.0, 28.0, 250.0, 0, 0x43200000}, // 250 x 160
  {LINE, 570.0, 28.0, 457.69, -83, 0x01},
  {LINE, 1024.0, 106.0, 353.91, 57, 0x01},
  {LINE, 728.0, 300.0, 116.02, 1, 0x01},
  {LINE, 726.0, 416.0, 288.40, -143, 0x01},
  {LINE, 900.0, 186.0, 329.54, -18, 0x01},
  {LINE, 1024.0, 600.0, 261.30, 28, 0x01},
  {LINE, 900.0, 830.0, 360.56, 124, 0x01},
  {LINE, 600.0, 630.0, 200.00, 90, 0x01},
  {LINE, 400.0, 630.0, 92.20, 41, 0x01},
  {LINE, 200.0, 1000.0, 829.46, -97, 0x01},
  {CIRCLE, 565, 800, 120},
  {LINE, 726.0, 416.0, 444.81, 73, 0x01},
  {LINE, 300.0, 544.0, 263.70, 158, 0x01},
  {LINE, 200.0, 300.0, 403.50, 8, 0x01},
  {LINE, 147.0, 700.0, 710.50, -113, 0x01},
  {LINE, 800.0, 420.0, 156.20, -140, 0x01},
  {LINE, 900.0, 300.0, 308.06, -13, 0x01},
  {LINE, 970.0, 600.0, 122.07, 35, 0x01},
  {LINE, 900.0, 700.0, 427.20, 111, 0x01},
  {RECTANGLE, 80.0, 700.0, 180.0, -60, 0x43340000}, // 180 x 180
  {CIRCLE, 710, 165, 90},
  {LINE, 700.0, 255.0, 145.77, 6, 0x01},
  {LINE, 685.0, 400.0, 393.22, 78, 0x01},
  {LINE, 300.0, 480.0, 470.74, 168, 0x01},
  {LINE, 200.0, 20.0, 190.07, -92, 0x01},
  {LINE, 390.0, 15.0, 300.00, 180, 0x01},
  {LINE, 390.0, 315.0, 305.37, -93, 0x01},
  {LINE, 20.0, 20.0, 180.00, -90, 0x01},
  {LINE, 200.0, 20.0, 805.23, 14, 0x01},
  {CIRCLE, 343, 860, 110},
  {LINE, 147.0, 700.0, 164.34, -69, 0x01},
  {LINE, 980.0, 900.0, 534.00, 90, 0x01}
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
