/* vim: ft=cpp:ts=2:sts=2:expandtab
 * A line segment to be rotated.
 */
#pragma once
#include "stdint.h"
#include "defines.h"
#include "structs.h"

constexpr rectangle_t kVehicle = {
  {kBoardWidth - 10.0, kBoardHeight - 10.0}, 10.0, 90, 5.0};

constexpr circle_t kTarget = {{202.5, 202.5}, 5.0};

// Enter the obstacles here. The format used for the `.item` part of 
// `obstacle_t` is {float, float, float, int16_t, uint32_t}. The last entry
// represents a `float` for the `rectangle_t` obstacle. In order to
// find the corresponding uint32_t representation, 
// use the [IEEE-754 Floating Point Converter]
// (https://www.h-schmidt.net/FloatConverter/IEEE754.html)
const obstacle_t PROGMEM obstacles[] = {
  // Borders of board
  {LINE, 0.0, 0.0, kBoardWidth, -90, 0x01},  // North
  {LINE, 0.0, 0.0, kBoardHeight, 0, 0x01},  // West
  {LINE, 0.0, kBoardHeight, kBoardWidth, -90, 0x01},  // South
  {LINE, kBoardWidth, 0.0, kBoardHeight, 0, 0x01},  // East
  // TODO: Obstacles
  {CIRCLE, 410.0, 220.0, 150.0}, // 1
  {RECTANGLE, 594.0, 586.0, 300.0, 30, 0x43160000}, // 2 (w=150.0)
  {LINE, 860, 75, 400, 50, 0x01},  // 3
  {LINE, 590, 350, 300, -30, 0x01},  // 4
  {LINE, 720, 830, 300, 300, 0x01},  // 5
  {LINE, 890, 730, 300, -30, 0x01},  // 6
  {LINE, 890, 730, 300, -30, 0x01},  // 6
  {LINE, 965, 504, 300, 0, 0x01},  // 7
  {LINE, 965, 504, 720, 225, 0x01},  // 8
  {LINE, 997, 557, 140, -4, 0x01},  // 9
  {LINE, 997, 557, 140, 245, 0x01},  // 10
  {RECTANGLE, 1040.0, 830.0, 100.0, -60, 0x42960000}, // 11 (w=75.0)
  {CIRCLE, 1220.0, 730.0, 100.0}, // 12
  {LINE, 1075, 1024, 140, 210, 0x01},  // 13
  {CIRCLE, 1118.0, 792.0, 50.0}, // 14
  {LINE, 1150, 894, 300, 200, 0x01},  // 15
  {LINE, 1155, 508, 120, -50, 0x01},  // 16
  {LINE, 1247, 585, 300, 240, 0x01},  // 17
  {LINE, 1252.6, 612.1, 300, 240, 0x01},  // 18
  {LINE, 1530, 1000, 337, 162, 0x01},  // 19
  {LINE, 1506.8, 435, 347.5, -45, 0x01},  // 20
  {LINE, 1512.4, 462.4, 308, -45, 0x01},  // 21
  {RECTANGLE, 1530.0, 700.0, 300.0, -90, 0x43960000}, // 22 (w=700.0)
  {LINE, 1425.8, 680, 304, 90, 0x01},  // 23
  {LINE, 1752.5, 680, 295.5, -90, 0x01},  // 24
  {LINE, 1850, 700, 300, 0, 0x01},  // 25
  {LINE, 1850, 700, 198, -90, 0x01},  // 26
  {LINE, 1850, 1000, 198, -90, 0x01},  // 27
  // Test obstacles
};
