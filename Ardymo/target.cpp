// Functions for the target object
#include "target.h"
#include "shapes.h"
#include "utils.h"

Target InitTarget(void) {
  obstacle_t obst;

  get_obstacle(&obst, 0);
  return Target(obst);
}

Vec Target::Heading(SensorValues*  sensors) {
  // Compute angle with respect to vehicle heading
  Vec tgt_heading = (p - sensors->position).rotate(180 - sensors->alpha);
  return tgt_heading.normalized().as_point();
}
