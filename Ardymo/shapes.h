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
  VEHICLE_TR1024 = 2, // Bottom right
  VEHICLE_TL = 3  // Top Left
};

constexpr uint8_t t_ix {0}; // Index of target in obstacle Array
constexpr uint8_t o_ix {0}; // Index of first obstacle in obstacle Array

// Three vehicle locations: 
constexpr rectangle_t vehicleLocation[] = {
  {{2038.0, 1014.0}, 10.0, 90, 5.0},
  {{1014.0, 1014.0}, 10.0, 90, 5.0},
  {{1014.0, 10.0}, 10.0, 0, 5.0},
  {{10.0, 10.0}, 10.0, -90, 5.0}
};

constexpr vehicle_name vehicleChoice[kLevels] = {
  VEHICLE_BR1024, VEHICLE_BR1024, VEHICLE_BR2048, VEHICLE_BR2048, VEHICLE_TL
};


// Enter the obstacles here. The format used for the `.item` part of
// `obstacle_t` is {float, float, float, int16_t, uint32_t}. The last entry
// represents a `float` for the `rectangle_t` obstacle. In order to
// find the corresponding uint32_t representation,
// use the [IEEE-754 Floating Point Converter]
// (https://www.h-schmidt.net/FloatConverter/IEEE754.html)

// 1. The first (0th) element of the array must be the target.
// 2. The second element must be the rectangle of the four borders
//    Rectangles origins are the **Top Right Corner**.
//    The extent by default is in the **negative x-direction**.
//    For example: The corners of the rectangle {4, 10, 5, 0, 3}
//    are: TR: (4, 10), BR: (4, 15), TL: (1, 10), BL: (1, 15)
//    A rotated rectangle: The corners of the rectangle {4, 10, 5, -90, 3}
//    are: TR: (9, 10), BR: (9, 13), TL: (4, 10), BL: (4, 13)

// Training: One obstacle, only borders and target. Learn to navigate
// around an obstacle and read sensors.
const obstacle_t PROGMEM _level0[] = {
  // Target: Must be element  zero of the obstacles array
  {TARGET, 202.5, 202.5, 5.0},
  // Borders of board
  {BORDER, 1024.0, 0.0, 1024.0, 0, 0x44800000},  // 1024 x 1024
  // Sole obstacle for Training
  {CIRCLE, 500.0, 500.0, 400.0},
};

// Easy: Easy to navigate around obstacles
const obstacle_t PROGMEM _level1[] = {
  // Target: Must be element  zero of the obstacles array
  {TARGET, 912.5, 102.5, 5.0},
  // 9 obstacles from "../doc/odg/map-easy.svg"
  // Representing 162 bytes
  /* *** Don't forget to put the target first manually *** */
  {BORDER, 1024.0, 0.0, 1024.0, 0, 0x44800000},
  {CIRCLE, 128, 350, 128},
  {CIRCLE, 764, 900, 100},
  {LINE, 1020.0, 512.0, 920.00, 90, 0x01},
  {LINE, 512.0, 536.0, 488.00, 0, 0x01},
  {LINE, 1024.0, 768.0, 494.00, 90, 0x01},
  {LINE, 1020.0, 192.0, 920.00, 90, 0x01},
  {CIRCLE, 700, 128, 64}
};

// Medium: More complex ways
const obstacle_t PROGMEM _level2[] = {
  // Target: Must be element  zero of the obstacles array
  {TARGET, 202.5, 202.5, 5.0},
  // Obstacles
  // 12 obstacles from "../doc/odg/map-medium.svg"
  // Representing 216 bytes
  /* *** Don't forget to put the target first manually *** */
  {BORDER, 2048.0, 0.0, 1024.0, 0, 0x45000000},
  {CIRCLE, 750, 220, 150},
  {CIRCLE, 1110, 220, 150},
  {CIRCLE, 580, 500, 150},
  {CIRCLE, 930, 500, 150},
  {CIRCLE, 200, 790, 150},
  {RECTANGLE, 370.0, 400.0, 150.0, 0, 0x43af0000},
  {RECTANGLE, 930.0, 140.0, 150.0, 135, 0x43160000},
  {LINE, 930.0, 650.0, 374.00, 0, 0x01},
  {LINE, 197.5, 0.0, 435.61, -23, 0x01},
  {LINE, 1000.0, 1000.0, 1391.59, -135, 0x01},
};

// Hard: Dead ends included
const obstacle_t PROGMEM _level3[] = {
  // Target: Must be element  zero of the obstacles array
  {TARGET, 64, 512, 5},
  // 13 obstacles from "../doc/odg/map-hard.svg"
  // Representing 234 bytes
  /* *** Don't forget to put the target first manually *** */
  {BORDER, 1024.0, 0.0, 1024.0, 0, 0x44800000},
  {RECTANGLE, 400.0, 16.0, 480.0, 0, 0x43c00000},
  {LINE, 0.0, 600.0, 141.4, -135, 0x01},
  {RECTANGLE, 71.6, 696.1, 550.0, 50, 0x42f00000},
  {LINE, 500.0, 0.0, 1029.6, -11, 0x01},
  {RECTANGLE, 1024.0, 100.0, 300.0, 0, 0x43d40000},
  {CIRCLE, 800, 600, 180},
  {LINE, 615.6, 579.3, 519.3, 97, 0x01},
  {LINE, 100.0, 517.0, 431.9, 11, 0x01},
  {LINE, 700.0, 780.0, 262.3, -22, 0x01},
  {CIRCLE, 471.706, 401.078, 39.323},
  {LINE, 415.0, 3.0, 352.0, 0, 0x01},
};

// Ultra: Doable. Needs lots of patience
const obstacle_t PROGMEM _level4[] = {
  // 39 obstacles from "../doc/odg/map-ultra.svg"
  // Representing 702 bytes
  /* *** Don't forget to put the target first manually *** */
  // Target: Must be element zero of the obstacles array
  {TARGET, 250.0, 500.0, 5.0},
  {BORDER, 1024.0, 0.0, 1024.0, 0, 0x44800000},  // 1024 x 1024
  // Obstacles
  {RECTANGLE, 570.0, 28.0, 250.0, 0, 0x43200000}, // 250 x 160
  {LINE, 570.0, 28.0, 457.69, -83, 0x01},
  {LINE, 1024.0, 106.0, 353.91, 57, 0x01},
  {LINE, 728.0, 300.0, 116.02, 1, 0x01},
  {LINE, 726.0, 416.0, 288.40, -143, 0x01},
  {LINE, 900.0, 186.0, 425.91, -14, 0x01},
  {LINE, 1024.0, 600.0, 261.30, 28, 0x01},
  {LINE, 900.0, 830.0, 360.56, 124, 0x01},
  {LINE, 600.0, 630.0, 200.00, 90, 0x01},
  {LINE, 400.0, 630.0, 92.20, 41, 0x01},
  {LINE, 200.0, 1000.0, 829.46, -97, 0x01},
  {CIRCLE, 565, 800, 120},
  {LINE, 726.0, 416.0, 444.81, 73, 0x01},
  {LINE, 300.0, 544.0, 358.24, 164, 0x01},
  {LINE, 200.0, 200.0, 502.80, 6, 0x01},
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
  {LINE, 390.0, 15.0, 300.00, 0, 0x01}, // Had angle 180. Check svg_parser.py
  {LINE, 390.0, 315.0, 305.37, -93, 0x01},
  {LINE, 20.0, 20.0, 180.00, -90, 0x01},
  {LINE, 200.0, 20.0, 805.23, 14, 0x01},
  {CIRCLE, 343, 860, 110},
  {LINE, 147.0, 700.0, 164.34, -69, 0x01},
  {LINE, 980.0, 900.0, 534.00, 90, 0x01},
  {LINE, 147.0, 700.0, 233.75, -149, 0x01},
  {LINE, 268.0, 500.0, 209.69, 163, 0x01},
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
