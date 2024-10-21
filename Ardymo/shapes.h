/* vim: ft=cpp:ts=2:sts=2:expandtab
 * A line segment to be rotated.
 */
#pragma once
#include "stdint.h"
#include "defines.h"
#include "structs.h"

const rectangle_t kVehicle = {
  {kBoardWidth - 10.0, kBoardHeight - 10.0}, 5.0, 90, 0.5};

const circle_t kTarget = {{10.0, 10.0}, 2.0};

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
  {CIRCLE, 100.0, 200.0, 20.0},
  {RECTANGLE, 400.0, 400.0, 50.0, 45, 0x3e99999a}, // Approximately 0.3
  {CIRCLE, 1600.0, 400.0, 40.0},
  // Test obstacles
  // Close to vehicle
  {RECTANGLE, kBoardWidth - 200, kBoardHeight - 50, 50.0, 90, 0x3dcccccd},//0.1
  {CIRCLE, kBoardWidth - 300.0, kBoardHeight - 200.0, 50.0},
};
