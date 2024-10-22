#pragma once
#include "objects.h"
#include "structs.h"

struct Target {
  Target() : p(Vec(0, 0)), r(1.0) {}
  Target(circle_t c) : p(Vec(c.p)), r(c.r) {}

  // Angle between vehicle heading and direction of target from vehicle
  // As coordinates, ready for display in the compass.
  Vec Heading(SensorValues* sensors); 
  float Distance(Vec source); // Distance to source
  Vec p;
  float r;
};

Target InitTarget(void);

// vim: ft=cpp
