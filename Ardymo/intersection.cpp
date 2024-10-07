/* vim: ts=2:sts=2:expandtab
 *
 * Implementation of intersection and distance functions
 * If the intersection is found, the variable X is set to the point
 * of intersection. We don't use pointers as return values. Takes too much
 * overhead.
 *
 */
#include "math.h"
#include "intersection.h"

static Vec X[2]; // Intersection points if they exists
constexpr static float epsilon = 1E-7;

uint8_t intersects_line(line_t sensor, line_t line);
uint8_t intersects_circle(line_t sensor, circle_t circle);
uint8_t intersects_rectangle(line_t sensor, rectangle_t rect);
line_t to_line(Vec p, Vec v);

/* det(v_0, v_1): Determinant of 2-dimensional matrix composed of two column
 * vectors v_0 and v_1.
 */
float determinant(point v0, point v1) {
  return v0.x * v1.y - v0.y * v1.x;
}

Vec intersect_point(uint8_t i) {
  return X[i];
}

uint8_t intersects(line_t sensor, obstacle obst) {

  switch (obst.type) {

    case LINE:
      return intersects_line(sensor, obst.line);
      break;

    case CIRCLE:
      return intersects_circle(sensor, obst.circle);
      break;

    case RECTANGLE:
      return intersects_rectangle(sensor, obst.rectangle);
      break;

    default:
      return 0;
  }
}

float distance(Vec origin, uint8_t i) {
  return sqrt(origin * X[i]);
}

uint8_t intersects_line(line_t sensor, line_t line) {
  float det; // determinant
  float tau; // scalar determining intersection point
  Vec ps_minus_pl;

  det = -determinant(sensor.v, line.v); // = det(sensor.v, -line.v)
  // Are the lines collinear?
  if (abs(det) < epsilon) {
    // Yes. Are (p_sig - p_l) and v_l collinear?
    ps_minus_pl = Vec(sensor.p) - Vec(line.p);
    if (abs(determinant(ps_minus_pl.as_point(), line.v))
          < epsilon) {
      return 0;
    }


  }

  tau = (sensor.v.x*(line.p.y - sensor.p.y) \
        - sensor.v.y*(line.p.x - sensor.p.x)) / det;

  // If this is a segment, the intersection must be within the segment.
  if (line.seg && ((tau < 0) || (tau > 1))) {
    return 0;
  }

  // Compute intersection point
  X[0] = Vec(line.v) * tau + line.p ;

  return 1;
}

uint8_t intersects_circle(line_t sensor, circle_t circle) {

  Vec psig = Vec(sensor.p);
  Vec vsig = Vec(sensor.v);
  Vec pc = Vec(circle.p);
  Vec psigc = psig - pc;
  float b2 = vsig * psigc; // b2 = b/2
  float a = vsig.sq();
  // b4ac = (b/2)^2 - ac:
  float b4ac = b2 * b2 - a * (psigc.sq() - circle.r * circle.r);

  if (b4ac < 0) {
    return 0;
  }

  // xi_+ = (-b2 + sqrt(b4ac))/a
  X[0] = psig + vsig * ((-b2 + sqrt(b4ac)) / a);

  // One intersection only?
  if (b4ac < epsilon) {
    return 1;
  }

  X[1] = psig + vsig * ((-b2 - sqrt(b4ac)) / a);
  return 2;
}

uint8_t intersects_rectangle(line_t sensor, rectangle_t rect) {
  Vec v = Vec(rect.v);
  Vec p = Vec(rect.p);
  Vec side;
  uint8_t n_int; // Number of intersections
  Vec intersection; // Temporary storage
  line_t rect_side = {rect.p, rect.v, 1};

  // Left side
  n_int = intersects_line(sensor, rect_side);
  if (n_int) {
    // Make a copy of X[0] which will be overwritten
    // if there is a second intersection
    X[1] = X[0];
  }

  // Front
  side = v.rotate(90) * rect.mu;
  p += v;
  rect_side = to_line(p, side);
  n_int += intersects_line(sensor, rect_side);
  if (n_int == 2) {
    return 2;
  }

  // Right side
  p += side;
  rect_side = to_line(p, -v);
  n_int += intersects_line(sensor, rect_side);
  if (n_int == 2) {
    return 2;
  }

  // Back
  p += -v;
  rect_side = to_line(p, -side);
  n_int += intersects_line(sensor, rect_side);

  return n_int;
}

line_t to_line(Vec p, Vec v) {
  line_t line = {p.as_point(), v.as_point(), 1};
  return line;
}
