/* vim: ft=cpp:ts=2:sts=2:expandtab
 * A line segment to be rotated.
 */

#include "defines.h"
#include "structs.h"

#define ARROW_LENGTH 7
// This is a small triangle used as an arrow point.
const PROGMEM point arrow[ARROW_LENGTH] = {
  {0, -2},
  {0, 0},
  {0, 2},
  {1, -1},
  {1, 0},
  {1, 1},
  {2, 0}
};

