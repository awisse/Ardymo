// Functions for the target object
#include "target.h"
#include "shapes.h"

Target InitTarget(void) {
  return Target(kTarget);
}

Vec Target::Heading(SensorValues*  sensors) {
  // Compute angle with respect to vehicle heading
  Vec tgt_heading = (p - sensors->position).rotate(180 - sensors->alpha);
  return tgt_heading.normalized().as_point();
}
