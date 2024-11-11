// Functions for the target object
#include "target.h"
#include "shapes.h"
#include "utils.h"

Target initTarget(uint8_t level) {
  obstacle_t obst;

  get_obstacle(&obst, level, t_ix);
  return Target(obst);
}
