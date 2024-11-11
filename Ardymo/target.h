#pragma once
#include "objects.h"
#include "structs.h"

struct Target {
  Target() : p(Vec(0, 0)), r(1.0) {}
  Target(circle_t c) : p(Vec(c.p)), r(c.r) {}
  Target(obstacle_t c) : p(Vec(c.item.circle.p)), r(c.item.circle.r) {}
  Vec p;
  float r;
};

Target initTarget(uint8_t target);
// vim: ft=cpp
