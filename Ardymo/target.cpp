// Functions for the target object
#include "target.h"
#include "shapes.h"

Target InitTarget(void) {
  return Target(kTarget);
}

Vec Target::Heading(Vec source) {
  return (p - source).normalized().as_point();
}
