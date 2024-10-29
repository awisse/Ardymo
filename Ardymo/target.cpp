// Functions for the target object
#include "target.h"
#include "shapes.h"
#include "utils.h"

Target InitTarget(void) {
  obstacle_t obst;

  get_obstacle(&obst, 0);
  return Target(obst);
}
