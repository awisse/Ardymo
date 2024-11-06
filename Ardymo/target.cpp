// Functions for the target object
#include "target.h"
#include "shapes.h"
#include "utils.h"

Target InitTarget(uint8_t level) {
  obstacle_t obst;

  get_obstacle(&obst, level, 0);
  return Target(obst);
}
