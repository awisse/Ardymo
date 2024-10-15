/* vim: ft=cpp:ts=2:sts=2:expandtab
 *
 * All functions to compute intersection and distance.
 */
#pragma once
#include "objects.h"
#include "structs.h"

// Returns number of intersections found
uint8_t intersects(line_t sensor, obstacle obst);
// Distance from origin to intersection points computed in intersection.cpp
// i = {0,1}: Which of the two intersection points to compare to
float distance(Vec origin, uint8_t i);
// Point of intersection found (i = {0,1})
Vec intersect_point(uint8_t i);
// Collision of rectangle with an object
side_t collides(rectangle_t rect, obstacle obst);
