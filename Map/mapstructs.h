/* vim: ft=cpp:ts=2:sts=2:expandtab
 *
 * Structs and classes used for map display
 */
#pragma once
#include <stdint.h>
#include "structs.h"

struct ScreenPt {
  int16_t x, y;
  ScreenPt(int16_t x, int16_t y) : x(x), y(y) {}
  ScreenPt(float x, float y) : x((int16_t)x), y((int16_t)y) {}
  ScreenPt() = default;
};
