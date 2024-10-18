/* vim: ft=cpp:ts=2:sts=2:expandtab
 * A line segment to be rotated.
 */
#pragma once
#include "defines.h"
#include "structs.h"

const rectangle_t kVehicle = {
  {kBoardWidth - 10.0, kBoardHeight - 10.0}, 5.0, 90, 0.5};

const circle_t kTarget = {{10.0, 10.0}, 2.0};

const uint16_t kObstCount = 4;
const obstacle PROGMEM obstacles[kObstCount] = {
  // Borders of board
  {LINE, {0.0, 0.0, kBoardWidth, -90, true}},  // North
  {LINE, {0.0, 0.0, kBoardHeight, 0, true}},  // West
  {LINE, {0.0, kBoardHeight, kBoardWidth, -90, true}},  // South
  {LINE, {kBoardWidth, 0.0, kBoardHeight, 0, true}}  // East
  // TODO: Obstacles
};
